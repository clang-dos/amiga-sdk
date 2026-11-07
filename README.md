# amiga-sdk

An SDK for classic AmigaOS programs (Kickstart 1.3 to 3.x, every 68k CPU)
built with clang from [clang-dos/llvm](https://github.com/clang-dos/llvm)
(`--target=m68k-amigaos`). It has:

- the C library: [picolibc](https://github.com/clang-dos/picolibc) with
  an AmigaOS layer (startup code, `malloc`, files, time), and the
  compiler-rt builtins, all built for the 68000;
- the AmigaOS headers, compatible with Hyperion's NDK 3.2, built from the
  AROS headers with the SDK's changes and additions (see
  [os/README.md](os/README.md));
- `libamiga.a`: a stub for every OS library function, and the amiga.lib,
  debug.lib, ddebug.lib and reaction.lib helper functions;
- examples, and scripts to run programs in vamos and FS-UAE.

Everything here can be redistributed: no file comes from the NDK, which
is used only to check that the SDK's headers match it.
`tools/amiga-ndk.py` can still install the NDK's own headers in their
place, for those who want them.

## Using it

The clang-dos toolchain package has the sysroot in `m68k-amigaos/`, next
to `bin/`, with the C library, the AmigaOS headers and `libamiga.a`.
Then:

    clang --target=m68k-amigaos -O2 hello.c -o hello
    clang --target=m68k-amigaos -mcpu=68020 -O2 window.c -o window

The driver links `crt0.o`, `libc.a`, `libamiga.a` and
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

Include `<proto/exec.h>`, `<proto/dos.h>`,
`<proto/intuition.h>` and so on, as with other Amiga compilers. The
calls go through `libamiga.a`, whose stubs move the arguments into the
library's registers and call it through its base. `SysBase` and
`DOSBase` are set up by the C library; open other libraries yourself and
set their base variable (`IntuitionBase`, `GfxBase`, ...), as in
`examples/window.c`.

`proto/*.h` declare the library base and include `clib/*_protos.h`;
there is no inline assembly. clang has no register parameters, so
function pointer types with register arguments (hooks, for example) are
ordinary C ones.

### Link library functions

`libamiga.a` also has the helper functions of the classic link
libraries, written for this SDK (`os/alib`):

- amiga.lib (`<clib/alib_protos.h>`):
  - hooks and BOOPSI: `HookEntry`, `CallHookA`/`CallHook`,
    `DoMethodA`/`DoMethod`, `DoSuperMethodA`/`DoSuperMethod`,
    `CoerceMethodA`/`CoerceMethod`, `SetSuperAttrs`. For a hook written in
    C, put `HookEntry` in `h_Entry` and the C function in `h_SubEntry`; it
    is called as `func(hook, object, message)`;
  - exec: `NewList`, `CreatePort`/`DeletePort`, `CreateExtIO`/`DeleteExtIO`,
    `CreateStdIO`/`DeleteStdIO`, `CreateTask`/`DeleteTask`, `BeginIO`,
    `LibCreatePool` and friends (exec's pools from V39, a simple version
    before);
  - `TimeDelay`, `DoTimer`, `FastRand`, `RangeRand`, `ACrypt`,
    `AddTOF`/`RemTOF` (a C function called at every vertical blank) and
    `waitbeam` (open graphics.library as `GfxBase`);
  - tool types (`ArgArrayInit`, `ArgArrayDone`, `ArgString`, `ArgInt`; open
    icon.library as `IconBase`) and commodities (`HotKey`, `InvertString`,
    `FreeIEvents`; open commodities.library as `CxBase`);
- debug.lib (`<clib/debug_protos.h>`): `KPrintF`, `KPutStr`, `KPutChar`,
  `KGetChar`, `KGetNum`, ... on the serial port, through exec's raw I/O,
  so they work anywhere, even with interrupts off;
- ddebug.lib (`<clib/ddebug_protos.h>`): the same as `DPrintF` and so on,
  on the parallel port;
- reaction.lib (`<clib/reaction_lib_protos.h>`): `ClickTabs`,
  `ChooserLabels`, `RadioButtons`, `BrowserNodes` and their `Free*`,
  `LBAddNode`/`LBEditNode`/`LBRemNode`, `GetAttrs`, `GetCode`,
  `LibDoGadgetMethod` and `OpenLayoutWindowTags`. Open the class library
  each one uses (`ClickTabBase`, ..., `LayoutBase`).

`ACrypt`, `FastRand` and `RangeRand` compute what AmigaOS's do, as in
AROS (AROS Public License); the rest is written for this SDK.
`examples/alibtest.c` checks them. Not there yet: the ARexx variable
functions (`CheckRexxMsg`, `GetRexxVar`, `SetRexxVar`) and the FFP
conversions (see [os/README.md](os/README.md)).

## Building the sysroot

    BIN=/path/to/clang-dos/bin LLVM_SRC=/path/to/llvm \
    PICOLIBC_SRC=/path/to/picolibc ./build-sysroot.sh m68k-amigaos

builds picolibc and the builtins (with meson, ninja and cmake), and the
AmigaOS headers and `libamiga.a` (with `tools/aros-ndk.py`), into
`m68k-amigaos/include`, `m68k-amigaos/fd` and `m68k-amigaos/lib`. The
AROS headers are fetched by `tools/fetch-aros.sh` at a tested commit
(about 30 MB, headers only), or set `AROS_SRC` to an AROS checkout. Set
`NDK=NDK3.2.lha` to install the NDK's headers instead. The clang-dos
toolchain package (clang-dos/dos-sdk) runs this script.

## Testing

- `vamos hello` (from `pip install amitools machine68k==0.3.0`) runs CLI
  programs without a ROM.
- `tools/fsuae-test.sh` boots a Workbench disk in FS-UAE under Xvfb with a
  host folder as `DH1:` and takes a screenshot. Kickstart ROMs and
  Workbench disks are your own and are never committed.
  With `SERIAL=/dev/pts/N` the Amiga's serial port goes to that terminal,
  where debug.lib's output appears.
- `python3 -m unittest discover -s tests` checks the stub generator and
  the LHA reader; with `AMIGA_SDK_NETWORK=1` it also installs the NDK
  headers from Aminet.
- `tools/ndk-compare.py` compares the SDK's headers with an NDK you have
  (see [os/README.md](os/README.md)).

The examples run in FS-UAE on Kickstart 1.3 (A500), 2.04 (A3000) and 3.1
(A500 and A1200), with Workbench 1.3, 2.x and 3.1.

## Not done yet

C++ (libc++), register-argument functions (libraries, devices),
FPU code generation, small data (a4), inline OS calls without the stubs,
auto-opening of libraries, Ctrl-C handling.

## License

Copyright (C) clang-retro contributors.

The clang-retro Amiga SDK is free software under the GNU Affero General Public
License, version 3 only (`AGPL-3.0-only`); the full text is in
[LICENSE](LICENSE). If you distribute a program built with it, or let
people use a modified version over a network, you must offer the
program's complete source under the same license.

The parts taken from AROS keep the AROS Public License (see
[os/README.md](os/README.md)): the headers in `os/include/`,
`os/patches/`, the prototypes in `os/functions/`, `os/alib/acrypt.c`,
and the random number functions in `os/alib/exec.c`.

Each contributor keeps the copyright in their own work and contributes
it under the same license; see [CONTRIBUTING.md](CONTRIBUTING.md).
