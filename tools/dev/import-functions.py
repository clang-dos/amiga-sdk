#!/usr/bin/env python3
"""Write os/functions/*.def, the SDK's table of OS library functions.

    import-functions.py AROS_SRC NDK_DIR OUTDIR

A developer tool, run by hand when the tables are refreshed; the SDK build
only reads the .def files. For every library of NDK 3.2 it takes:

  - the function offsets (LVOs) and argument registers of NDK 3.2, which
    are the AmigaOS ABI: a call through any other offset or register would
    go wrong on a real Amiga;
  - the C prototype from AROS's function list (rom/.../*.conf), and from
    the NDK only for the functions AROS does not have, or where AROS's
    takes different arguments.

Private functions and AROS-only functions (which AmigaOS does not have at
those offsets) are left out.
"""
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import confparse  # noqa: E402
import sfdparse  # noqa: E402

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
for _n in ["bevel", "bitmap", "button", "checkbox", "chooser", "clicktab", "drawlist",
           "fuelgauge", "getfile", "getfont", "getscreenmode", "glyph", "integer", "label",
           "layout", "listbrowser", "palette", "penmap", "radiobutton", "requester",
           "scroller", "slider", "space", "speedbar", "string", "window"]:
    CONF[_n] = "workbench/classes/reaction/%s/%s.conf" % (_n, _n)


def norm(s):
    s = re.sub(r"\s+", " ", s).strip()
    s = re.sub(r"\s*\*\s*", " *", s)
    s = s.replace("( ", "(").replace(" )", ")").replace(" ,", ",")
    return s


def nargs(args):
    a = args.strip()
    if not a or a == "void" or a == "VOID":
        return 0
    depth, n = 0, 1
    for ch in a:
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        elif ch == "," and depth == 0:
            n += 1
    return n


def main():
    aros, ndk, out = sys.argv[1:4]
    # Functions whose AROS prototype takes other types than the NDK's (from
    # ndk-compare.py --protos --json): those take the NDK's types.
    prefer_ndk = set()
    if len(sys.argv) > 4:
        import json
        with open(sys.argv[4]) as f:
            for h, diffs in json.load(f).items():
                if h.startswith("clib/"):
                    for d in diffs:
                        m = re.match(r"(\w+): ", d)
                        if m:
                            prefer_ndk.add(m.group(1))
                        elif d in ("does not compile",):
                            prefer_ndk.add("*" + h[5:-len("_protos.h")])
    commit = os.popen("git -C %s rev-parse HEAD" % aros).read().strip()
    os.makedirs(out, exist_ok=True)
    sfddir = os.path.join(ndk, "SFD")
    stats = []
    for fn in sorted(os.listdir(sfddir)):
        if not fn.endswith("_lib.sfd"):
            continue
        lib = fn[:-len("_lib.sfd")]
        sfd = sfdparse.parse_sfd(os.path.join(sfddir, fn))
        pub = [f for f in sfd["funcs"] if f["public"]]
        if not pub or not sfd["base"]:
            continue
        aros_funcs = {}
        if lib in CONF:
            _, _, funcs = confparse.parse_conf(os.path.join(aros, CONF[lib]))
            aros_funcs = {f["name"]: f for f in funcs}
        lines = []
        src = "AROS %s (%s)" % (CONF[lib], commit[:12]) if lib in CONF else None
        lines.append("# %s: the functions of %s." % (lib + ".def", sfd["libname"] or lib))
        lines.append("# Offsets and registers: the AmigaOS ABI (as in NDK 3.2).")
        if src:
            lines.append("# Prototypes: from %s, except those marked ndk." % src)
        lines.append("# Generated by tools/dev/import-functions.py; see os/README.md.")
        lines.append("base %s" % sfd["base"])
        lines.append("basetype %s" % norm(sfd["basetype"] or "struct Library *").rstrip(" *"))
        lines.append("libname %s" % (sfd["libname"] or lib))
        for inc in sfd["includes"]:
            lines.append("include %s" % inc)
        n_aros = n_ndk = 0
        for f in pub:
            a = aros_funcs.get(f["name"])
            mark = ""
            if (a and f["kind"] == "func" and nargs(a["args"]) == nargs(f["args"])
                    and f["name"] not in prefer_ndk and "*" + lib not in prefer_ndk):
                ret, args = a["ret"], a["args"]
                n_aros += 1
            else:
                ret, args = f["ret"], f["args"]
                mark = "  # ndk"
                n_ndk += 1
            proto = norm("%s %s(%s)" % (ret, f["name"], args or "void"))
            regs = ",".join(f["regs"])
            if f["kind"] == "func":
                ver = " v%d" % f["version"] if f["version"] else ""
                lines.append("%d%s %s (%s)%s" % (f["lvo"], ver, proto, regs, mark))
            elif f["kind"] == "varargs":
                lines.append("= %s%s" % (proto, mark))
            else:
                lines.append("alias %s%s" % (proto, mark))
        with open(os.path.join(out, lib + ".def"), "w") as fo:
            fo.write("\n".join(lines) + "\n")
        stats.append((lib, n_aros, n_ndk))
    for s in stats:
        print("%-20s aros %3d  ndk %3d" % s)


if __name__ == "__main__":
    main()
