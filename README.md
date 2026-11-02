# amiga-sdk

Startup code and OS call stubs for `clang --target=m68k-amigaos`, the classic
AmigaOS target in [clang-dos/llvm](https://github.com/clang-dos/llvm). This is
phase 0: enough to build and run CLI programs on Kickstart 1.3 and later.

## Build a sysroot

The NDK can't be redistributed, so the sysroot is built from your copy. The
NDK 3.1 on the Amiga Developer CD 1.2 works (`NDK/NDK_3.1/Includes&Libs`).

    NDK="/path/to/NDK_3.1/Includes&Libs" CLANG_BIN=/path/to/clang/bin \
      ./build-sysroot.sh /path/to/m68k-amigaos

This writes `include/` (NDK headers plus generated `proto/*.h`), `lib/crt0.o`,
`lib/libamiga.a` (a stub for each of the ~1000 public OS functions, generated
from the `.fd` files by `tools/fd2stub.py`) and a placeholder `lib/libc.a`
with memcpy, memset, memmove, memcmp and strlen.

The compiler-rt builtins for m68k (`__mulsi3`, `__divsi3`, 64-bit and soft
float) go in `<clang resource dir>/lib/m68k-unknown-amigaos/libclang_rt.builtins.a`.

## Use it

    clang --target=m68k-amigaos --sysroot=/path/to/m68k-amigaos -O2 hello.c -o hello

The driver links with `ld.lld --emit-relocs` and converts the result to a Hunk
executable with `llvm-elf2hunk`. `-mcpu=68020` and so on select the CPU;
68000 code also runs on later CPUs.

## Test

- `vamos hello` (from `pip install amitools machine68k==0.3.0`) runs CLI
  programs without a ROM.
- `tools/fsuae-test.sh` boots a Workbench disk in FS-UAE under Xvfb with a
  host folder as `DH1:` and takes a screenshot. Kickstart ROMs and Workbench
  disks are the user's own and are never committed.

Run `python3 -m unittest discover -s tests` to check the stub generator.

## Not done yet

picolibc AmigaOS layer (stdio, malloc), Workbench icon start with a real
program, register-argument functions (hooks, libraries), FPU code, small
data (a4), C++.
