#!/usr/bin/env python3
"""Generate stack-to-register call stubs (an amiga.lib) from .fd files.

Each public library function in an .fd file becomes one assembly file:
the stub takes C stack arguments, loads them into the registers the .fd
file names, loads the library base into a6 and calls the function's
vector. Usage: fd2stub.py OUTDIR file.fd...
"""
import os
import re
import sys

CALLEE_SAVED = {"d2", "d3", "d4", "d5", "d6", "d7", "a2", "a3", "a4", "a5", "a6"}
REG_ORDER = ["d0", "d1", "d2", "d3", "d4", "d5", "d6", "d7",
             "a0", "a1", "a2", "a3", "a4", "a5", "a6"]


def parse_fd(path):
    base, bias, public = None, 30, True
    funcs = []
    with open(path, encoding="latin-1") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("*"):
                continue
            if line.startswith("##"):
                cmd = line[2:].split()
                if cmd[0] == "base":
                    base = cmd[1].lstrip("_")
                elif cmd[0] == "bias":
                    bias = int(cmd[1])
                elif cmd[0] == "public":
                    public = True
                elif cmd[0] == "private":
                    public = False
                elif cmd[0] == "end":
                    break
                continue
            m = re.match(r"(\w+)\(([^)]*)\)\(([^)]*)\)", line)
            if m:
                name, args, regs = m.groups()
                regs = [r.lower() for r in re.split(r"[/,]", regs) if r]
                if public:
                    funcs.append((name, regs, -bias))
            bias += 6
    return base, funcs


def stub(base, name, regs, lvo):
    saved = sorted({r for r in regs if r in CALLEE_SAVED} | {"a6"},
                   key=REG_ORDER.index)
    n = 4 * len(saved)
    out = [f"\t.section .text.{name},\"ax\",@progbits",
           f"\t.globl {name}", f"\t.type {name},@function", f"{name}:"]
    out.append(f"\tlea (-{n},%sp),%sp")
    out.append(f"\tmovem.l {'/'.join('%' + r for r in saved)},(%sp)")
    for i, r in enumerate(regs):
        out.append(f"\tmove.l ({n + 4 + 4 * i},%sp),%{r}")
    out.append(f"\tmove.l {base},%a6")
    out.append(f"\tjsr ({lvo},%a6)")
    out.append(f"\tmovem.l (%sp),{'/'.join('%' + r for r in saved)}")
    out.append(f"\tlea ({n},%sp),%sp")
    out.append("\trts")
    out.append(f"\t.size {name},.-{name}")
    return "\n".join(out) + "\n"


def main():
    outdir = sys.argv[1]
    os.makedirs(outdir, exist_ok=True)
    for fd in sys.argv[2:]:
        base, funcs = parse_fd(fd)
        for name, regs, lvo in funcs:
            if "a6" in regs:
                continue  # needs a6 for an argument; none of the C APIs do
            with open(os.path.join(outdir, name + ".s"), "w") as f:
                f.write(stub(base, name, regs, lvo))


if __name__ == "__main__":
    main()
