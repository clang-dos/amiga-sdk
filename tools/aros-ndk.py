#!/usr/bin/env python3
"""Build the SDK's AmigaOS headers and libamiga.a from the AROS sources.

    aros-ndk.py --aros AROS_SRC --out SYSROOT [--bin DIR] [--no-stubs]

Writes SYSROOT/include (the AROS headers for AmigaOS 3.x, with the SDK's
changes in os/), SYSROOT/include/{clib,proto} and SYSROOT/fd from the
function tables in os/functions, and SYSROOT/lib/libamiga.a: a stub for
each function, which moves the C arguments into the registers the library
takes and calls it through its base.
"""
import argparse
import concurrent.futures
import os
import re
import shutil
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
SDK = os.path.dirname(HERE)

# compiler/include directories that AROS generates or that are AROS-only.
SKIP_TOP = {"clib", "proto", "defines", "inline", "pragmas", "aros", "asm", "c++",
            "vhi", "scsi", "irda", "bluetooth", "midi", "libcore", "SDI"}
# Module trees whose public headers belong to the AmigaOS API.
MODULE_ROOTS = ["rom", "workbench/libs", "workbench/devs", "workbench/classes",
                "workbench/c", "arch/m68k-amiga", "arch/m68k-all"]
MODULE_SKIP = re.compile(r"/(hidds?|bluetooth|usb|network|filesys|fs|mui|zune|"
                         r"AHI|poseidon|gallium|mesa|vulkan|efi|acpi|pci)(/|$)", re.I)


def copy_tree(src, dst, skip=()):
    for dp, dns, fns in os.walk(src):
        rel = os.path.relpath(dp, src)
        top = rel.split(os.sep)[0]
        if top in skip:
            dns[:] = []
            continue
        for fn in fns:
            if fn.endswith((".h", ".i")):
                d = os.path.join(dst, rel)
                os.makedirs(d, exist_ok=True)
                shutil.copy(os.path.join(dp, fn), os.path.join(d, fn))


def module_includes(aros, out):
    """Copy the headers AROS installs with %copy_includes path=P dir=D."""
    done = []
    for root in MODULE_ROOTS:
        for dp, dns, fns in os.walk(os.path.join(aros, root)):
            if MODULE_SKIP.search(dp.replace(os.sep, "/")):
                dns[:] = []
                continue
            if "mmakefile.src" not in fns:
                continue
            with open(os.path.join(dp, "mmakefile.src"), encoding="latin-1") as f:
                text = f.read().replace("\\\n", " ")
            for m in re.finditer(r"^%copy_includes\b(.*)$", text, re.M):
                opts = dict(re.findall(r"(\w+)=(\S+)", m.group(1)))
                path = opts.get("path", "")
                if "$" in path or "$" in opts.get("dir", ""):
                    continue
                if path in ("hidd", "mui", "zune", "usb", "aros", "oop", "gallium",
                            "cybergraphx", "clib"):
                    continue
                src = os.path.normpath(os.path.join(dp, opts.get("dir", "include")))
                if not os.path.isdir(src):
                    continue
                files = opts.get("includes")
                for fn in (files.strip('"').split() if files and "$" not in files
                           else os.listdir(src)):
                    if fn.endswith(".h") and os.path.isfile(os.path.join(src, fn)):
                        d = os.path.join(out, path)
                        os.makedirs(d, exist_ok=True)
                        shutil.copy(os.path.join(src, fn), os.path.join(d, fn))
                        done.append(os.path.join(path, fn))
    return done


# --- function tables ---------------------------------------------------------

def read_def(path):
    lib = {"name": os.path.basename(path)[:-4], "includes": [], "funcs": []}
    with open(path) as f:
        for line in f:
            line = line.split("#")[0].strip()
            if not line:
                continue
            key, _, rest = line.partition(" ")
            if key in ("base", "basetype", "libname"):
                lib[key] = rest.strip()
            elif key == "nobase":
                lib["base"] = None
            elif key == "include":
                lib["includes"].append(rest.strip())
            elif key in ("=", "alias"):
                proto = rest.strip()
                base = lib["funcs"][-1]
                lib["funcs"].append(dict(base, proto=proto, kind=key,
                                         name=proto_name(proto)))
            else:
                m = re.match(r"(-\d+)(?:\s+v(\d+))?\s+(.*\))\s*\(([^)]*)\)$", line)
                if not m:
                    sys.exit("%s: cannot read: %s" % (path, line))
                regs = [r for r in m.group(4).split(",") if r]
                lib["funcs"].append(dict(lvo=int(m.group(1)), proto=m.group(3), regs=regs,
                                         kind="func", name=proto_name(m.group(3))))
    return lib


def proto_name(proto):
    return re.match(r".*?\b(\w+)\s*\(", proto).group(1)


def write_fd(lib, path):
    out = ["* %s: generated from os/functions/%s.def" % (os.path.basename(path), lib["name"])]
    if lib["base"]:
        out.append("##base _%s" % lib["base"])
    bias = None
    for f in lib["funcs"]:
        if f["kind"] != "func":
            continue
        if bias is None:
            bias = -f["lvo"]
            out += ["##bias %d" % bias, "##public"]
        if bias < -f["lvo"]:
            out.append("##private")
            while bias < -f["lvo"]:
                out.append("%sPrivate%d()()" % (lib["name"], bias))
                bias += 6
            out.append("##public")
        args = re.match(r".*?\((.*)\)$", f["proto"]).group(1)
        names = []
        for i, a in enumerate(split_args(args)):
            if a.strip() in ("void", "VOID", "..."):
                continue
            m = re.search(r"\(\s*\*\s*(\w+)", a) if "(*" in a else \
                re.search(r"(\w+)\s*(?:\[\s*\])?\s*$", a)
            names.append(m.group(1) if m else "arg%d" % (i + 1))
        out.append("%s(%s)(%s)" % (f["name"], ",".join(names), "/".join(f["regs"])))
        bias += 6
    out.append("##end")
    with open(path, "w") as fo:
        fo.write("\n".join(out) + "\n")


def split_args(args):
    parts, depth, cur = [], 0, ""
    for ch in args:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        if ch == "," and depth == 0:
            parts.append(cur.strip())
            cur = ""
        else:
            cur += ch
    if cur.strip():
        parts.append(cur.strip())
    return parts


def write_clib(lib, path):
    guard = "CLIB_%s_PROTOS_H" % lib["name"].upper()
    out = ["#ifndef %s" % guard, "#define %s" % guard, "/*",
           "    The functions of %s (generated from os/functions/%s.def)." %
           (lib.get("libname", lib["name"]), lib["name"]), "*/", "",
           "#ifndef EXEC_TYPES_H", "#include <exec/types.h>", "#endif"]
    for inc in lib["includes"]:
        out.append("#include <%s>" % inc)
    out += ["", "#ifdef __cplusplus", "extern \"C\" {", "#endif", ""]
    for f in lib["funcs"]:
        out.append("%s;" % f["proto"])
    out += ["", "#ifdef __cplusplus", "}", "#endif", "", "#endif /* %s */" % guard, ""]
    with open(path, "w") as fo:
        fo.write("\n".join(out))


def write_proto(lib, path):
    guard = "PROTO_%s_H" % lib["name"].upper()
    with open(path, "w") as fo:
        fo.write("""#ifndef {g}
#define {g}
/* {lib}: its functions (which link to the stubs in libamiga.a) and its base. */
#include <clib/{name}_protos.h>
{decl}#endif /* {g} */
""".format(g=guard, lib=lib.get("libname", lib["name"]), name=lib["name"],
           decl="#ifndef __NOLIBBASE__\nextern %s *%s;\n#endif\n" % (
               lib.get("basetype", "struct Library"), lib["base"]) if lib["base"] else ""))


CALLEE_SAVED = {"d2", "d3", "d4", "d5", "d6", "d7", "a2", "a3", "a4", "a5", "a6"}
REG_ORDER = ["d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
             "a0", "a1", "a2", "a3", "a4", "a5", "a6"]


def stub(base, f):
    """A stub that loads the stack arguments into registers and calls the
    function; for a varargs function, the last register gets the address of
    the arguments from there on (the tag list or the format arguments)."""
    regs = f["regs"]
    saved = sorted({r for r in regs if r in CALLEE_SAVED} | {"a6"}, key=REG_ORDER.index)
    n = 4 * len(saved)
    name = f["name"]
    out = ["\t.section .text.%s,\"ax\",@progbits" % name, "\t.globl %s" % name,
           "\t.type %s,@function" % name, "%s:" % name,
           "\tlea (-%d,%%sp),%%sp" % n,
           "\tmovem.l %s,(%%sp)" % "/".join("%" + r for r in saved)]
    for i, r in enumerate(regs):
        if f["kind"] == "=" and i == len(regs) - 1:
            if r.startswith("a"):
                out.append("\tlea (%d,%%sp),%%%s" % (n + 4 + 4 * i, r))
            else:
                out.append("\tmove.l %%sp,%%%s" % r)
                out.append("\tadd.l #%d,%%%s" % (n + 4 + 4 * i, r))
        else:
            out.append("\tmove.l (%d,%%sp),%%%s" % (n + 4 + 4 * i, r))
    if "a6" not in regs:
        out.append("\tmove.l %s,%%a6" % base)
    out += ["\tjsr (%d,%%a6)" % f["lvo"],
            "\tmovem.l (%%sp),%s" % "/".join("%" + r for r in saved),
            "\tlea (%d,%%sp),%%sp" % n, "\trts", "\t.size %s,.-%s" % (name, name)]
    return "\n".join(out) + "\n"


def write_functions(sysroot, bindir, stubs):
    inc = os.path.join(sysroot, "include")
    fddir = os.path.join(sysroot, "fd")
    for d in (os.path.join(inc, "clib"), os.path.join(inc, "proto"), fddir):
        os.makedirs(d, exist_ok=True)
    srcs = {}
    defdir = os.path.join(SDK, "os", "functions")
    for fn in sorted(os.listdir(defdir)):
        if not fn.endswith(".def"):
            continue
        lib = read_def(os.path.join(defdir, fn))
        write_fd(lib, os.path.join(fddir, lib["name"] + "_lib.fd"))
        write_clib(lib, os.path.join(inc, "clib", lib["name"] + "_protos.h"))
        write_proto(lib, os.path.join(inc, "proto", lib["name"] + ".h"))
        for f in lib["funcs"]:
            srcs[f["name"]] = stub(lib["base"], f)
    if not stubs:
        return
    exe = ".exe" if os.name == "nt" else ""
    clang = os.path.join(bindir, "clang" + exe) if bindir else "clang"
    llvm_ar = os.path.join(bindir, "llvm-ar" + exe) if bindir else "llvm-ar"
    work = tempfile.mkdtemp(prefix="aros-ndk-")
    try:
        def build(item):
            name, text = item
            s = os.path.join(work, name + ".s")
            o = os.path.join(work, name + ".o")
            with open(s, "w") as f:
                f.write(text)
            subprocess.run([clang, "--target=m68k-amigaos", "-c", s, "-o", o], check=True)
            return o
        with concurrent.futures.ThreadPoolExecutor(os.cpu_count() or 4) as pool:
            objs = list(pool.map(build, sorted(srcs.items())))
        libdir = os.path.join(sysroot, "lib")
        os.makedirs(libdir, exist_ok=True)
        lib = os.path.join(libdir, "libamiga.a")
        if os.path.exists(lib):
            os.remove(lib)
        rsp = os.path.join(work, "objs.rsp")
        with open(rsp, "w") as f:
            f.write("\n".join('"%s"' % o.replace("\\", "/") for o in objs))
        subprocess.run([llvm_ar, "rcs", lib, "@" + rsp], check=True)
        print("wrote %s (%d stubs)" % (lib, len(objs)))
    finally:
        shutil.rmtree(work, ignore_errors=True)


# --- headers ------------------------------------------------------------------

def write_headers(aros, out, patches=True):
    if os.path.exists(out):
        shutil.rmtree(out)
    os.makedirs(out)
    copy_tree(os.path.join(aros, "compiler/include"), out, SKIP_TOP)
    copy_tree(os.path.join(aros, "compiler/arossupport/include"), os.path.join(out, "aros"))
    copy_tree(os.path.join(aros, "compiler/include/aros"), os.path.join(out, "aros"))
    copy_tree(os.path.join(aros, "arch/m68k-all/include/aros"), os.path.join(out, "aros/m68k"))
    for d in ("hardware",):
        copy_tree(os.path.join(aros, "arch/m68k-amiga/include", d), os.path.join(out, d))
    shutil.copy(os.path.join(aros, "compiler/include/exec/execbase.inc"),
                os.path.join(out, "exec/execbase.h"))
    n = module_includes(aros, out)
    # The SDK's changes to AROS's headers, for AmigaOS and NDK 3.2.
    pdir = os.path.join(SDK, "os", "patches")
    for fn in sorted(os.listdir(pdir)) if os.path.isdir(pdir) else []:
        if fn.endswith(".patch"):
            r = subprocess.run(["patch", "-p1", "-s", "-N", "-d", out, "-i",
                                os.path.join(pdir, fn)], capture_output=True, text=True)
            if r.returncode != 0:
                sys.exit("%s does not apply:\n%s%s" % (fn, r.stdout, r.stderr))
    copy_tree(os.path.join(SDK, "os", "include"), out)
    print("installed the AROS headers in %s (%d from modules)" % (out, len(n)))


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--aros", required=True, help="AROS source tree")
    ap.add_argument("--out", required=True, help="the sysroot")
    ap.add_argument("--bin", help="directory with clang and llvm-ar")
    ap.add_argument("--no-stubs", action="store_true")
    args = ap.parse_args()
    write_headers(args.aros, os.path.join(args.out, "include"))
    write_functions(args.out, args.bin, not args.no_stubs)


if __name__ == "__main__":
    main()
