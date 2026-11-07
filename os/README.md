# The AmigaOS headers and libamiga.a

`tools/aros-ndk.py` builds the SDK's AmigaOS interface from the AROS
sources and the files in this folder. The result is meant to be a drop-in
for Hyperion's NDK 3.2 when compiling with clang: the same header names,
structures, constants, prototypes, library base names and function
offsets. All of it can be redistributed.

## What is here

- `patches/amigaos.patch`: changes to AROS's headers for AmigaOS, made
  by hand. They remove AROS-only parts, use the AmigaOS types and values
  where AROS differs, and add what AROS lacks (constants, structure
  fields, old names).
- `include/`: headers written for the SDK, copied over the patched AROS
  ones. They are the headers AROS has no version of (ReAction classes such
  as `gadgets/texteditor.h`, `dos/shell.h`, the battery memory bits, ...),
  `clib/alib_protos.h` and the other link library prototypes, and a few
  AROS support headers.
- `functions/*.def`: one table per library or device: base name and type,
  and for each function its offset, the registers of its arguments and its
  C prototype. They were started from AROS's function lists and corrected
  by hand. `aros-ndk.py` writes `clib/*_protos.h`, `proto/*.h`, `fd/*.fd`
  and a call stub for each function in `libamiga.a` from them.
- `alib/`: the amiga.lib, debug.lib, ddebug.lib and reaction.lib helper
  functions, written for the SDK. Each source file is one object in
  `libamiga.a`, so a program only links what it uses.

## Provenance and licences

Nothing here is copied or generated from the NDK or from any other
Commodore, Amiga Inc. or Hyperion file. The AROS headers are under the
AROS Public License; `patches/amigaos.patch` and the prototypes taken over
into `functions/*.def` are changes to them and are under that licence too.
The sysroot gets a copy of it as `include/LICENSE.AROS`. Everything else in
this folder is the SDK's own work, under the repository's licence (AGPL-3.0-only).

The NDK is used only to check the result. `tools/ndk-compare.py` compiles
every NDK 3.2 header and the SDK's one with clang and compares structure
layouts, constant values, prototypes, base names, fd offsets and
registers. It reads an NDK that each developer installs for themselves.
The NDK is never stored in this repository.

    python3 tools/ndk-compare.py --ref NDK/include --new SYSROOT/include \
      --protos --fd-ref NDK/FD --fd-new SYSROOT/fd

At the tested AROS commit (see `tools/fetch-aros.sh`) it finds no
differences in the 223 headers and libraries it compares.

## Kickstart versions

The headers describe AmigaOS 3.2, as the NDK does. A program must not
call a function its Kickstart's library does not have yet, so it opens
each library with the lowest version it needs, as with any other
compiler. The `libamiga.a` helpers run on Kickstart 1.3 and later. The
BOOPSI ones need 2.04 (V36), and the reaction.lib ones need the ReAction
classes. `LibDoGadgetMethodA()` uses
`DoGadgetMethodA()` on V39 and later, and calls the gadget directly on
V37. They are tested in FS-UAE on Kickstart 1.3, 2.04 and 3.1 with
`examples/alibtest.c`.

## Not provided

- The Motorola FFP conversions of amiga.lib (`afp`, `arnd`, `dbf`, `fpa`,
  `fpbcd`): they work on FFP numbers, which clang does not use. They are
  declared, but not in `libamiga.a`.
- The ARexx variable functions (`CheckRexxMsg`, `GetRexxVar`,
  `SetRexxVar`). They are declared, but not in `libamiga.a` yet.
- The 1.x amiga.lib stdio functions: `clib/alib_stdio_protos.h` is empty,
  as in NDK 3.2. Use the C library.
