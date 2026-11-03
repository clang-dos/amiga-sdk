"""Read an NDK .sfd file (only for checking and filling gaps, never shipped)."""
import re


def parse_sfd(path):
    lib = {"base": None, "basetype": None, "libname": None, "includes": [], "funcs": []}
    bias = 30
    public = True
    version = None
    pending_varargs = pending_alias = False
    buf = ""
    with open(path, encoding="latin-1") as f:
        lines = f.read().split("\n")
    for raw in lines:
        line = raw.rstrip()
        if not buf and (not line.strip() or line.startswith("*")):
            continue
        if not buf and line.startswith("=="):
            cmd, _, arg = line[2:].partition(" ")
            arg = arg.strip()
            if cmd == "base":
                lib["base"] = arg.lstrip("_")
            elif cmd == "basetype":
                lib["basetype"] = arg
            elif cmd == "libname":
                lib["libname"] = arg
            elif cmd == "include":
                lib["includes"].append(arg.strip("<>"))
            elif cmd == "bias":
                bias = int(arg)
            elif cmd == "public":
                public = True
            elif cmd == "private":
                public = False
            elif cmd == "reserve":
                bias += 6 * int(arg)
            elif cmd == "version":
                version = int(arg)
            elif cmd == "varargs":
                pending_varargs = True
            elif cmd == "alias":
                pending_alias = True
            elif cmd == "end":
                break
            continue
        buf += " " + line.strip()
        m = re.match(r"\s*(.*?)\(([A-Za-z0-9,/ -]*)\)\s*$", buf)
        if not m or buf.count("(") < 2:
            continue
        proto, regs = m.group(1).strip(), m.group(2)
        buf = ""
        regs = [r.strip().lower() for r in re.split(r"[,/-]", regs) if r.strip()]
        pm = re.match(r"(.*?)\b(\w+)\s*\((.*)\)\s*$", proto)
        ret, name, args = pm.group(1).strip(), pm.group(2), pm.group(3).strip()
        entry = dict(name=name, ret=re.sub(r"\s+", " ", ret),
                     args=re.sub(r"\s+", " ", args), regs=regs, public=public,
                     version=version)
        if pending_varargs or pending_alias:
            entry["kind"] = "varargs" if pending_varargs else "alias"
            entry["lvo"] = lib["funcs"][-1]["lvo"] if lib["funcs"] else -bias
            pending_varargs = pending_alias = False
        else:
            entry["kind"] = "func"
            entry["lvo"] = -bias
            bias += 6
        lib["funcs"].append(entry)
    return lib
