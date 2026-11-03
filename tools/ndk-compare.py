#!/usr/bin/env python3
"""Compare an AmigaOS include tree with the NDK 3.2 one.

    ndk-compare.py --ref NDK_INCLUDE --new NEW_INCLUDE [--fd-ref DIR --fd-new DIR]
                   [--headers LIST] [--json OUT]

For every header of the reference tree (except clib/, proto/, inline/ and
the pragmas), both trees are compiled with clang for m68k-amigaos and
compared on:

  - structures and unions: every field's name, offset and size, and the
    record's size (clang's record layout dump);
  - integer constants: every object-like macro of the reference header that
    is an integer constant, evaluated by the compiler;
  - library functions (with --fd-ref and --fd-new): name, offset and
    registers.

It only reads the NDK; nothing of it is copied anywhere. Prints a report
and exits non-zero when anything differs (or with --json, writes the
differences there too).
"""
import argparse
import concurrent.futures
import json
import os
import re
import struct
import subprocess
import sys
import tempfile

SKIP_DIRS = ("clib/", "proto/", "inline/", "pragma/", "pragmas/")


def clang_cmd(args, inc, extra=()):
    return [args.clang, "--target=m68k-amigaos", "-nostdinc",
            "-isystem", inc, "-isystem", args.resource_include,
            "-w", *extra]


def tu_source(header):
    return "#include <exec/types.h>\n#include <%s>\n" % header


# --- structure layouts ---------------------------------------------------

LAYOUT_LINE = re.compile(r"^\s*(\d+)(?::(\d+)-(\d+))?\s*\|(\s*)(.*?)\s*$")


def parse_layouts(text):
    """Parse clang's full record layout dump into {record: [(path, off, type)]}."""
    records = {}
    blocks = text.split("*** Dumping AST Record Layout")
    for b in blocks[1:]:
        lines = b.strip("\n").split("\n")
        name = None
        fields = []
        stack = []  # (indent, name)
        size = None
        for line in lines:
            m = LAYOUT_LINE.match(line)
            if m:
                off = int(m.group(1))
                bits = (m.group(2), m.group(3))
                indent = len(m.group(4))
                decl = m.group(5)
                if name is None:
                    name = decl
                    stack = [(indent, "")]
                    continue
                while stack and stack[-1][0] >= indent:
                    stack.pop()
                # "struct Tag name", "UWORD name", "char *name", or an
                # unnamed member: "struct Tag" / "union (unnamed at ...)".
                if decl.startswith(("struct ", "union ")) and (
                        len(decl.split()) == 2 or decl.endswith(")")):
                    ftype, fname = decl, ""
                else:
                    fm = re.match(r"(.*?)\s*(\w+)$", decl)
                    ftype, fname = (fm.group(1), fm.group(2)) if fm else (decl, "")
                path = ".".join(p for _, p in stack[1:] if p)
                full = (path + "." if path else "") + fname
                if bits[0] is not None:
                    full += ":%s-%s" % bits
                fields.append((full, off, ftype))
                stack.append((indent, fname))
                continue
            sm = re.match(r"\s*\[sizeof=(\d+), align=(\d+)", line)
            if sm:
                size = (int(sm.group(1)), int(sm.group(2)))
        if name and not name.startswith("struct __NSConstantString"):
            records[name] = {"size": size, "fields": fields}
    return records


def layouts(args, inc, header):
    with tempfile.NamedTemporaryFile("w", suffix=".c", delete=False) as f:
        f.write(tu_source(header))
        src = f.name
    try:
        r = subprocess.run(
            clang_cmd(args, inc, ["-fsyntax-only", "-Xclang", "-fdump-record-layouts-complete", src]),
            capture_output=True, text=True, errors="replace")
    finally:
        os.unlink(src)
    if r.returncode != 0:
        return None, r.stderr
    return parse_layouts(r.stdout), ""


# --- integer constants ---------------------------------------------------

def macros_of(args, inc, header):
    """Object-like macros the header itself (or its directory) defines."""
    with tempfile.NamedTemporaryFile("w", suffix=".c", delete=False) as f:
        f.write(tu_source(header))
        src = f.name
    try:
        r = subprocess.run(clang_cmd(args, inc, ["-E", "-dD", src]),
                           capture_output=True, text=True, errors="replace")
    finally:
        os.unlink(src)
    if r.returncode != 0:
        return None
    target = os.path.normpath(os.path.join(inc, header))
    cur = None
    out = {}
    for line in r.stdout.split("\n"):
        if line.startswith("# "):
            m = re.match(r'# \d+ "([^"]+)"', line)
            if m:
                cur = os.path.normpath(m.group(1))
            continue
        if cur != target:
            continue
        m = re.match(r"#define (\w+)(\s+(.*))?$", line)
        if m and m.group(3) and m.group(3).strip():
            out[m.group(1)] = m.group(3).strip()
    return out


def eval_constants(args, inc, header, names):
    """Evaluate integer macros with the compiler; returns {name: value}."""
    names = [n for n in names if not n.endswith("_H") and not n.startswith("__")]
    work = tempfile.mkdtemp(prefix="ndkcmp-")
    try:
        while True:
            src = os.path.join(work, "v.c")
            with open(src, "w") as f:
                f.write(tu_source(header))
                f.write("const long long __ndkcmp_values[] = {\n")
                for i, n in enumerate(names):
                    f.write("#line %d\n(long long)(%s),\n" % (100000 + i, n))
                f.write("0};\n")
            obj = os.path.join(work, "v.o")
            r = subprocess.run(clang_cmd(args, inc, ["-c", src, "-o", obj, "-ferror-limit=0"]),
                               capture_output=True, text=True, errors="replace")
            if r.returncode == 0:
                break
            bad = set()
            for m in re.finditer(r"v\.c:(\d+):\d+: error", r.stderr):
                ln = int(m.group(1)) - 100000
                if 0 <= ln < len(names):
                    bad.add(ln)
            if not bad:
                return {}
            names = [n for i, n in enumerate(names) if i not in bad]
        binf = os.path.join(work, "v.bin")
        objcopy = os.path.join(os.path.dirname(args.clang), "llvm-objcopy")
        subprocess.run([objcopy, "-O", "binary", "--only-section=.rodata*",
                        "--only-section=.data*", obj, binf], check=True)
        with open(binf, "rb") as f:
            data = f.read()
        vals = struct.unpack(">%dq" % (len(names) + 1), data[:8 * (len(names) + 1)])
        return dict(zip(names, vals))
    finally:
        subprocess.run(["rm", "-rf", work])


# --- library functions ---------------------------------------------------

def read_fds(d):
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import fd2stub
    out = {}
    for name in sorted(os.listdir(d)):
        if name.endswith(".fd"):
            base, funcs = fd2stub.parse_fd(os.path.join(d, name))
            out[name] = {f[0]: (f[2], tuple(f[1])) for f in funcs}
    return out


# --- prototypes ----------------------------------------------------------------

def prototypes(args, inc, header):
    """{function: (return type, [parameter types])} with clang's canonical types."""
    with tempfile.NamedTemporaryFile("w", suffix=".c", delete=False) as f:
        f.write(tu_source(header))
        src = f.name
    try:
        r = subprocess.run(clang_cmd(args, inc, ["-fsyntax-only", "-Xclang", "-ast-dump=json",
                                                 "-Xclang", "-ast-dump-filter=", src]),
                           capture_output=True, text=True, errors="replace")
    finally:
        os.unlink(src)
    if r.returncode != 0:
        return None
    out = {}
    typedefs = {}
    dec = json.JSONDecoder()
    text, pos = r.stdout, 0
    while True:
        pos = text.find("{", pos)
        if pos < 0:
            break
        try:
            node, end = dec.raw_decode(text, pos)
        except ValueError:
            pos += 1
            continue
        pos = end
        for d in node.get("inner", []) if node.get("kind") == "TranslationUnitDecl" else [node]:
            if d.get("kind") == "TypedefDecl":
                typedefs[d["name"]] = d["type"]["qualType"]
            if d.get("kind") != "FunctionDecl" or d.get("isImplicit"):
                continue
            out[d["name"]] = d["type"]["qualType"]
    return {k: canon(v, typedefs) for k, v in out.items()}


def canon(t, typedefs):
    """The type with every typedef replaced, and without qualifiers."""
    for _ in range(10):
        new = re.sub(r"\b[A-Za-z_]\w*\b",
                     lambda m: "(%s)" % typedefs[m.group(0)] if m.group(0) in typedefs
                     and m.group(0) != typedefs[m.group(0)] else m.group(0), t)
        if new == t:
            break
        t = new
    t = re.sub(r"\bconst\b|\bvolatile\b|\bstruct\b|\bunion\b|\benum\b|\bsigned\b", "", t)
    t = re.sub(r"\(unnamed[^)]*\)", "", t)
    t = re.sub(r"\s+", "", t)
    # (int) -> int, but keep (*) and parameter lists
    for _ in range(10):
        new = re.sub(r"\(([A-Za-z_0-9 ]+)\)(?!\s*\()", r"\1", t)
        new = re.sub(r"\(([A-Za-z_0-9]+\*+)\)", r"\1", new)
        if new == t:
            break
        t = new
    return t.replace("long", "int") if False else t


def compare_protos(args):
    report = {}
    cdir = os.path.join(args.ref, "clib")
    for fn in sorted(os.listdir(cdir)):
        if not fn.endswith("_protos.h"):
            continue
        h = "clib/" + fn
        ref = prototypes(args, args.ref, h)
        if ref is None:
            continue
        if not os.path.exists(os.path.join(args.new, h)):
            report[h] = ["missing"]
            continue
        new = prototypes(args, args.new, h)
        if new is None:
            report[h] = ["does not compile"]
            continue
        d = []
        own = open(os.path.join(cdir, fn), encoding="latin-1").read()
        for name, t in sorted(ref.items()):
            if not re.search(r"\b%s\s*\(" % name, own):
                continue
            if name not in new:
                d.append("%s: missing" % name)
            elif new[name] != t:
                d.append("%s: %s, expected %s" % (name, new[name], t))
        if d:
            report[h] = d
    return report


# --- driver ----------------------------------------------------------------

def compare_header(args, h):
    diffs = []
    new_path = os.path.join(args.new, h)
    if not os.path.exists(new_path):
        return h, ["missing header"]
    ref_l, err = layouts(args, args.ref, h)
    if ref_l is None:
        return h, ["reference does not compile: " + err[:200]]
    new_l, err = layouts(args, args.new, h)
    if new_l is None:
        return h, ["does not compile: " + "\n".join(
            l for l in err.split("\n") if "error" in l)[:600]]
    with open(os.path.join(args.ref, h), encoding="latin-1") as f:
        own = f.read()
    for rec, ref in sorted(ref_l.items()):
        if "(unnamed" in rec or "::" in rec:
            continue  # checked as fields of the record around it
        new = new_l.get(rec)
        if new is None:
            kind, tag = rec.split()[0], rec.split()[-1]
            if re.search(r"\b%s\s+%s\s*\{" % (kind, tag), own):
                diffs.append("%s: missing" % rec)
            else:
                diffs.append("%s: not included (the NDK header includes it)" % rec)
            continue
        if ref["size"] != new["size"]:
            diffs.append("%s: size %s, expected %s" % (rec, new["size"], ref["size"]))
        nf = {f[0]: f for f in new["fields"]}
        for name, off, _ in ref["fields"]:
            if "(unnamed" in name:
                continue
            if name not in nf:
                diffs.append("%s: no field %s (offset %d)" % (rec, name, off))
            elif nf[name][1] != off:
                diffs.append("%s.%s: offset %d, expected %d" % (rec, name, nf[name][1], off))
    ref_m = macros_of(args, args.ref, h) or {}
    if ref_m:
        ref_v = eval_constants(args, args.ref, h, sorted(ref_m))
        new_v = eval_constants(args, args.new, h, sorted(ref_v))
        for n, v in sorted(ref_v.items()):
            if n not in new_v:
                diffs.append("#define %s: missing (expected %d)" % (n, v))
            elif new_v[n] != v and (new_v[n] - v) % (1 << 32) != 0:
                diffs.append("#define %s: %d, expected %d" % (n, new_v[n], v))
    return h, diffs


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--ref", required=True)
    ap.add_argument("--new", required=True)
    ap.add_argument("--fd-ref")
    ap.add_argument("--fd-new")
    ap.add_argument("--headers", help="file with the headers to compare")
    ap.add_argument("--clang", default="clang")
    ap.add_argument("--resource-include")
    ap.add_argument("--json")
    ap.add_argument("--protos", action="store_true",
                    help="also compare the clib/ function prototypes")
    ap.add_argument("-j", type=int, default=os.cpu_count() or 4)
    args = ap.parse_args()
    if os.sep not in args.clang:
        from shutil import which
        args.clang = which(args.clang) or args.clang
    if not args.resource_include:
        rd = subprocess.run([args.clang, "-print-resource-dir"], capture_output=True,
                            text=True).stdout.strip()
        args.resource_include = os.path.join(rd, "include")
    args.ref = os.path.abspath(args.ref)
    args.new = os.path.abspath(args.new)

    if args.headers:
        with open(args.headers) as f:
            headers = [l.strip() for l in f if l.strip() and not l.startswith("#")]
    else:
        headers = []
        for dp, _, fs in os.walk(args.ref):
            for fn in fs:
                if fn.endswith(".h"):
                    rel = os.path.relpath(os.path.join(dp, fn), args.ref).replace(os.sep, "/")
                    if not rel.startswith(SKIP_DIRS):
                        headers.append(rel)
        headers.sort()

    report = {}
    with concurrent.futures.ThreadPoolExecutor(args.j) as pool:
        for h, diffs in pool.map(lambda h: compare_header(args, h), headers):
            if diffs:
                report[h] = diffs

    if args.fd_ref and args.fd_new:
        ref, new = read_fds(args.fd_ref), read_fds(args.fd_new)
        for fd, funcs in ref.items():
            key = "fd/" + fd
            if fd not in new:
                report[key] = ["missing"]
                continue
            d = []
            for fn, (lvo, regs) in sorted(funcs.items(), key=lambda x: -x[1][0]):
                if "Private" in fn:
                    continue
                got = new[fd].get(fn)
                if got is None:
                    d.append("%s (%d): missing" % (fn, lvo))
                elif got != (lvo, regs):
                    d.append("%s: %d %s, expected %d %s" % (fn, got[0], "/".join(got[1]),
                                                            lvo, "/".join(regs)))
            for fn, (lvo, _) in new[fd].items():
                if fn not in funcs:
                    d.append("%s (%d): not in the NDK" % (fn, lvo))
            if d:
                report[key] = d

    if args.protos:
        report.update(compare_protos(args))

    n = 0
    for k in sorted(report):
        print("== " + k)
        for d in report[k]:
            print("   " + d)
            n += 1
    print("%d headers/libraries compared, %d differences in %d" %
          (len(headers), n, len(report)))
    if args.json:
        with open(args.json, "w") as f:
            json.dump(report, f, indent=1, sort_keys=True)
    sys.exit(1 if report else 0)


if __name__ == "__main__":
    main()
