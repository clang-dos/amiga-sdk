#!/usr/bin/env python3
"""Write os/functions/<lib>.def from AROS's function lists.

    import-functions.py AROS_SRC INCLUDE_DIR OUTDIR LIB...

A developer tool for starting a library's table; the .def files are then
maintained by hand (the SDK build only reads them). It takes from AROS's
<lib>.conf the function names, offsets (from their slots), registers and
prototypes, leaves out private functions, and adds the varargs forms that
AROS's build makes (Foo for FooA with a tag list, FooTags for FooTagList).
INCLUDE_DIR is the SDK's installed headers, to check that the base type is
public. Where AmigaOS differs (checked with ndk-compare.py), the .def files
are corrected by hand.
"""
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import confparse  # noqa: E402

CONF = {
    "amigaguide": "workbench/libs/amigaguide/amigaguide.conf",
    "asl": "workbench/libs/asl/asl.conf",
    "battclock": "rom/battclock/battclock.conf",
    "battmem": "rom/battmem/battmem.conf",
    "bullet": "workbench/libs/bullet/bullet.conf",
    "cardres": "rom/card/card.conf",
    "colorwheel": "workbench/classes/gadgets/colorwheel/colorwheel.conf",
    "commodities": "workbench/libs/commodities/commodities.conf",
    "console": "rom/devs/console/console.conf",
    "datatypes": "workbench/libs/datatypes/datatypes.conf",
    "disk": "rom/disk/disk.conf",
    "diskfont": "workbench/libs/diskfont/diskfont.conf",
    "dos": "rom/dos/dos.conf",
    "exec": "rom/exec/exec.conf",
    "expansion": "rom/expansion/expansion.conf",
    "gadtools": "workbench/libs/gadtools/gadtools.conf",
    "graphics": "rom/graphics/graphics.conf",
    "icon": "workbench/libs/icon/icon.conf",
    "iffparse": "workbench/libs/iffparse/iffparse.conf",
    "input": "rom/devs/input/input.conf",
    "intuition": "rom/intuition/intuition.conf",
    "keymap": "rom/keymap/keymap.conf",
    "layers": "rom/hyperlayers/layers.conf",
    "locale": "workbench/libs/locale/locale.conf",
    "lowlevel": "workbench/libs/lowlevel/lowlevel.conf",
    "misc": "rom/misc/misc.conf",
    "nonvolatile": "workbench/libs/nonvolatile/nonvolatile.conf",
    "potgo": "arch/m68k-amiga/potgo/potgo.conf",
    "ramdrive": "workbench/devs/ramdrive/ramdrive.conf",
    "realtime": "workbench/libs/realtime/realtime.conf",
    "rexxsyslib": "workbench/libs/rexxsyslib/rexxsyslib.conf",
    "timer": "rom/timer/timer.conf",
    "translator": "workbench/libs/translator/translator.conf",
    "utility": "rom/utility/utility.conf",
    "wb": "workbench/libs/workbench/workbench.conf",
}
for _n in ["mathffp", "mathieeedoubbas", "mathieeedoubtrans", "mathieeesingbas",
           "mathieeesingtrans", "mathtrans"]:
    CONF[_n] = "workbench/libs/%s/%s.conf" % (_n, _n)
REACTION = ["bevel", "bitmap", "button", "checkbox", "chooser", "clicktab", "drawlist",
            "fuelgauge", "getfile", "getfont", "getscreenmode", "glyph", "integer", "label",
            "layout", "listbrowser", "palette", "penmap", "radiobutton", "requester",
            "scroller", "slider", "space", "speedbar", "string", "window"]
for _n in REACTION:
    CONF[_n] = "workbench/classes/reaction/%s/%s.conf" % (_n, _n)

RESOURCES = {"battclock", "battmem", "cardres", "disk", "misc", "potgo"}
IMAGES = {"bevel", "bitmap", "drawlist", "glyph", "label", "penmap"}
CLASSES = {"requester", "window"}
LIBNAME = {"wb": "workbench.library", "cardres": "card.resource",
           "rexxsyslib": "rexxsyslib.library"}


def kind(lib):
    path = CONF[lib]
    if "/devs/" in path or lib == "timer":
        return "device"
    if lib in RESOURCES:
        return "resource"
    return "library"


def libname(lib):
    if lib in LIBNAME:
        return LIBNAME[lib]
    if lib in REACTION:
        return lib + (".image" if lib in IMAGES else ".class" if lib in CLASSES else ".gadget")
    if lib == "colorwheel":
        return "colorwheel.gadget"
    return lib + "." + kind(lib)


def bias(lib, funcs):
    # Some lists start with the library's own Open/Close vectors.
    if kind(lib) == "resource" or (funcs and funcs[0]["slot"] == 0
                                   and re.match(r"(?i)open(lib)?$", funcs[0]["name"])):
        return 6
    return 42 if kind(lib) == "device" else 30


def public_struct(inc, name):
    """Is struct name defined in the installed headers?"""
    pat = re.compile(r"^\s*struct\s+%s\s*(\{|$)" % re.escape(name), re.M)
    for root, _, files in os.walk(inc):
        for f in files:
            if f.endswith(".h"):
                with open(os.path.join(root, f), encoding="latin-1") as fh:
                    if pat.search(fh.read()):
                        return True
    return False


def norm(s):
    s = re.sub(r"\s+", " ", s).strip()
    s = re.sub(r"\s*\*\s*", " *", s)
    return s.replace("( ", "(").replace(" )", ")").replace(" ,", ",")


def split_args(args):
    out, depth, cur = [], 0, ""
    for ch in args:
        if ch == "," and depth == 0:
            out.append(cur.strip()); cur = ""
            continue
        depth += ch == "("
        depth -= ch == ")"
        cur += ch
    if cur.strip():
        out.append(cur.strip())
    return out


VARARGS_NAMES = {"VFPrintf": "FPrintf", "VPrintf": "Printf", "VFWritef": "FWritef",
                 "EasyRequestArgs": "EasyRequest", "BuildEasyRequestArgs": "BuildEasyRequest"}


def varargs(f):
    """The varargs form AROS's build generates for f, as (name, args), or None."""
    name, args = f["name"], split_args(f["args"])
    if "novararg" in " ".join(f.get("flags", [])) or not args:
        return None
    last = args[-1]
    if name in VARARGS_NAMES:
        return VARARGS_NAMES[name], args[:-1] + ["..."]
    if "TagItem" in last:
        if name.endswith("TagList"):
            vname = name[:-len("TagList")] + "Tags"
        elif name.endswith("A"):
            vname = name[:-1]
        else:
            return None
        return vname, args[:-1] + ["Tag tag1", "..."]
    return None


def main():
    aros, inc, out = sys.argv[1:4]
    libs = sys.argv[4:] or sorted(CONF)
    commit = os.popen("git -C %s rev-parse HEAD" % aros).read().strip()
    for lib in libs:
        cfg, cdef, funcs = confparse.parse_conf(os.path.join(aros, CONF[lib]))
        base = cfg.get("libbase") or (lib[0].upper() + lib[1:] + "Base")
        btype = cfg.get("libbasetypeextern") or cfg.get("libbasetype") or ""
        m = re.match(r"struct\s+(\w+)", btype)
        default = "struct Device" if kind(lib) == "device" else "struct Library"
        if not m or not public_struct(inc, m.group(1)):
            btype = default
        lines = ["# %s.def: the functions of %s." % (lib, libname(lib)),
                 "# From AROS %s (%s), checked against the AmigaOS ABI with"
                 % (CONF[lib], commit[:12]),
                 "# tools/ndk-compare.py; maintained by hand. See os/README.md.",
                 "base %s" % base, "basetype %s" % norm(btype),
                 "libname %s" % libname(lib)]
        for inc_ in re.findall(r"#\s*include\s*<([^>]+)>", cdef):
            if not inc_.startswith(("aros/", "proto/", "clib/")):
                lines.append("include %s" % inc_)
        for f in funcs:
            if f["private"]:
                continue
            lvo = -(bias(lib, funcs) + 6 * f["slot"])
            ver = " v%d" % f["version"] if f["version"] else ""
            proto = norm("%s %s(%s)" % (f["ret"], f["name"], f["args"] or "void"))
            lines.append("%d%s %s (%s)" % (lvo, ver, proto, ",".join(f["regs"])))
            for a in f.get("alias", []):
                lines.append("alias " + norm("%s %s(%s)" % (f["ret"], a, f["args"] or "void")))
            va = varargs(f)
            if va:
                lines.append("= " + norm("%s %s(%s)" % (f["ret"], va[0], ", ".join(va[1]))))
        with open(os.path.join(out, lib + ".def"), "w") as fo:
            fo.write("\n".join(lines) + "\n")
        print("%-20s %d functions" % (lib, sum(1 for l in lines if l[:1] == "-")))


if __name__ == "__main__":
    main()
