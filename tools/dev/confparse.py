import re, sys
def parse_conf(path):
    text = open(path, encoding="latin-1").read()
    cfg = {}
    m = re.search(r"##begin config\n(.*?)##end config", text, re.S)
    if m:
        for line in m.group(1).split("\n"):
            line = line.split("#")[0].strip()
            if line:
                k, _, v = line.partition(" ")
                cfg.setdefault(k, v.strip())
    m = re.search(r"##begin cdef\n(.*?)##end cdef", text, re.S)
    cdef = m.group(1) if m else ""
    m = re.search(r"##begin functionlist\n(.*?)##end functionlist", text, re.S)
    funcs = []  # (slot, proto, name, regs, private, version)
    slot = 0; version = None
    for raw in (m.group(1).split("\n") if m else []):
        line = raw.strip()
        if not line:
            slot += 1  # an empty line is an unused vector
            continue
        if line.startswith("#"):
            continue
        if line.startswith("."):
            parts = line.split("#")[0].split()
            if parts[0] == ".skip":
                slot += int(parts[1])
            elif parts[0] == ".private" and funcs:
                funcs[-1]["private"] = True
            elif parts[0] == ".version":
                version = int(parts[1])
            elif parts[0] == ".alias" and funcs:
                funcs[-1].setdefault("alias", []).append(parts[1])
            elif parts[0] in (".function", ".novararg", ".unusedlibbase", ".cfunction", ".nolibbase"):
                funcs and funcs[-1].setdefault("flags", []).append(parts[0])
            continue
        mm = re.match(r"(.*?)\b(\w+)\s*\((.*)\)\s*\(([^)]*)\)\s*$", line)
        if not mm:
            print("??", path, line, file=sys.stderr); slot += 1; continue
        ret, name, args, regs = mm.groups()
        regs = [r.strip().lower() for r in re.split(r"[,/]", regs) if r.strip()]
        funcs.append(dict(slot=slot, ret=ret.strip(), name=name, args=args.strip(), regs=regs,
                          private=False, version=version))
        slot += 1
    return cfg, cdef, funcs
