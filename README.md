# amiga-sdk

An SDK for classic AmigaOS programs (Kickstart 1.3 to 3.x, every 68k CPU)
built with clang from [clang-dos/llvm](https://github.com/clang-dos/llvm)
(`--target=m68k-amigaos`). It has:

- the C library: [picolibc](https://github.com/clang-dos/picolibc) with
  an AmigaOS layer (startup code, `malloc`, files, time), and the
  compiler-rt builtins, all built for the 68000;
- `tools/amiga-ndk.py`, which installs the AmigaOS headers and a stub for
  every OS library function from Hyperion's NDK 3.2;
- the amiga.lib helper functions (`os/alib`), which `amiga-ndk.py` adds
  to `libamiga.a`;
- examples, and scripts to run programs in vamos and FS-UAE.

Everything here can be redistributed. The NDK can't: it is free to
download from Aminet, but not to pass on, so `amiga-ndk.py` downloads it
for each user (or takes a copy you have) and installs it into the sysroot.

## Using it

The clang-dos toolchain package has the sysroot in `m68k-amigaos/`, next
to `bin/`. Install the NDK once:

    python3 share/clang-dos/amiga-ndk.py

This downloads `NDK3.2.lha` from Aminet (8 MB, checked against a known
SHA-256), and writes `m68k-amigaos/include` and
`m68k-amigaos/lib/libamiga.a`. Pass `--archive NDK3.2.lha` to use a copy
you have, or `--sysroot DIR` for another sysroot. It needs only Python 3:
the LHA archive is unpacked by `tools/lha.py`.

Then:

    clang --target=m68k-amigaos -O2 hello.c -o hello
    clang --target=m68k-amigaos -mcpu=68020 -O2 window.c -o window

The driver links `crt0.o`, `libc.a`, `libamiga.a` (when installed) and
the builtins with `ld.lld --emit-relocs`, and converts the result to a
Hunk executable with `llvm-elf2hunk`. Code is for the 68000 unless
`-mcpu=` says otherwise; the libraries are 68000 code, which runs on
every CPU.

### C programs

- `main(argc, argv)` gets the CLI arguments, split as the CLI does. Started
  from Workbench, `argc` is 0 and `argv` is the `struct WBStartup` message;
  `stdin`/`stdout`/`stderr` then open a console window when first used.
- `malloc` takes memory from `AllocMem()`; what is left is freed at exit.
- `printf`, files (`fopen`, `open`, `stat`, `rename`, `mkdir`, ...) and
  time (`time`, `clock_gettime`, `nanosleep`) work as in POSIX, with
  AmigaOS paths (`DH0:dir/file`, `RAM:`).
- The CLI's default stack (4000 bytes on Kickstart 1.3) is small for
  `printf`: run `Stack 20000` first.

### OS calls

With the NDK installed, include `<proto/exec.h>`, `<proto/dos.h>`,
`<proto/intuition.h>` and so on, as with other Amiga compilers. The
calls go through `libamiga.a`, whose stubs move the arguments into the
library's registers and call it through its base. `SysBase` and
`DOSBase` are set up by the C library; open other libraries yourself and
set their base variable (`IntuitionBase`, `GfxBase`, ...), as in
`examples/window.c`.

`amiga-ndk.py` makes these changes for clang:

- `proto/*.h` are generated (prototypes from `clib/` and the library
  base), in place of the NDK's, which include gcc's inline assembly.
- clang has no register parameters, so the NDK's function pointer types
  with register arguments (hooks, for example) are declared as ordinary
  C ones; calling such a function needs inline assembly.

### amiga.lib functions

`libamiga.a` also has the helper functions of the classic `amiga.lib`,
written for this SDK (`os/alib`), with the prototypes of
`<clib/alib_protos.h>`:

- hooks and BOOPSI: `HookEntry`, `CallHookA`/`CallHook`,
  `DoMethodA`/`DoMethod`, `DoSuperMethodA`/`DoSuperMethod`,
  `CoerceMethodA`/`CoerceMethod`, `SetSuperAttrs`. For a hook written in
  C, put `HookEntry` in `h_Entry` and the C function in `h_SubEntry`; it
  is called as `func(hook, object, message)`;
- exec: `NewList`, `CreatePort`/`DeletePort`, `CreateExtIO`/`DeleteExtIO`,
  `CreateStdIO`/`DeleteStdIO`, `CreateTask`/`DeleteTask`, `BeginIO`,
  `LibCreatePool` and friends (exec's pools from V39, a simple version
  before);
- `TimeDelay`, `DoTimer`, `FastRand`, `RangeRand`, `ACrypt`;
- tool types (`ArgArrayInit`, `ArgArrayDone`, `ArgString`, `ArgInt`; open
  icon.library as `IconBase`) and commodities (`HotKey`, `InvertString`,
  `FreeIEvents`; open commodities.library as `CxBase`).

`ACrypt`, `FastRand` and `RangeRand` compute what AmigaOS's do, as in
AROS (AROS Public License); the rest is written for this SDK.
`examples/alibtest.c` checks them; it passes on Kickstart 1.3, 2.04 and
3.1 in FS-UAE. Not there yet: `AddTOF`/`RemTOF`, `waitbeam`, and the
ARexx variable functions (`CheckRexxMsg`, `GetRexxVar`, `SetRexxVar`).

## Building the sysroot

    BIN=/path/to/clang-dos/bin LLVM_SRC=/path/to/llvm \
    PICOLIBC_SRC=/path/to/picolibc ./build-sysroot.sh m68k-amigaos

builds picolibc and the builtins (with meson, ninja and cmake) into
`m68k-amigaos/include` and `m68k-amigaos/lib`. Set `NDK=NDK3.2.lha` to
install the NDK as well. The clang-dos toolchain package
(clang-dos/dos-sdk) runs this script.

## Testing

- `vamos hello` (from `pip install amitools machine68k==0.3.0`) runs CLI
  programs without a ROM.
- `tools/fsuae-test.sh` boots a Workbench disk in FS-UAE under Xvfb with a
  host folder as `DH1:` and takes a screenshot. Kickstart ROMs and
  Workbench disks are your own and are never committed.
- `python3 -m unittest discover -s tests` checks the stub generator and
  the LHA reader; with `AMIGA_SDK_NETWORK=1` it also installs the NDK
  headers from Aminet.

The examples run on Kickstart 1.3 (A500) and 3.1 (A1200) in FS-UAE.

## Not done yet

C++ (libc++), register-argument functions (libraries, devices),
FPU code generation, small data (a4), inline OS calls without the stubs,
auto-opening of libraries, Ctrl-C handling.
