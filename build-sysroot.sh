#!/bin/sh
# Build an m68k-amigaos sysroot for clang from the NDK you supply.
#
#   NDK=/path/to/NDK_3.1/Includes&Libs CLANG_BIN=/path/to/bin ./build-sysroot.sh OUT
#
# NDK is the "Includes&Libs" folder of the Native Developer Kit (3.1 from the
# Amiga Developer CD, or 3.2 from Hyperion): it needs include_h and fd. The
# NDK can't be redistributed, so each user builds their own sysroot.
#
# OUT gets include/ (the NDK headers plus proto/ headers), lib/crt0.o and
# lib/libamiga.a (startup code and a stub for every public OS function),
# and a placeholder lib/libc.a.
set -e
out=${1:?usage: build-sysroot.sh OUT}
: "${NDK:?set NDK to the NDK Includes&Libs folder}"
bin=${CLANG_BIN:+$CLANG_BIN/}
here=$(cd "$(dirname "$0")" && pwd)
cc="${bin}clang --target=m68k-amigaos -O2 -Wall -Wno-unused-command-line-argument"
work=$(mktemp -d)
trap 'rm -rf "$work"' EXIT

mkdir -p "$out/include" "$out/lib"
cp -R "$NDK/include_h/." "$out/include/"
find "$out/include" -name '*.info' -delete

# proto/<lib>.h: the C prototypes plus the library base they use.
mkdir -p "$out/include/proto"
for fd in "$NDK"/fd/*_lib.fd; do
  lib=$(basename "$fd" _lib.fd)
  base=$(sed -n 's/^##base _\{0,1\}\([A-Za-z0-9_]*\).*/\1/p' "$fd" | head -1)
  [ -f "$out/include/clib/${lib}_protos.h" ] || continue
  guard=$(echo "PROTO_${lib}_H" | tr 'a-z' 'A-Z')
  case $lib in exec) btype="struct ExecBase" ;; *) btype="struct Library" ;; esac
  cat > "$out/include/proto/$lib.h" <<HDR
#ifndef $guard
#define $guard
#include <clib/${lib}_protos.h>
#ifndef __NOLIBBASE__
extern $btype *$base;
#endif
#endif
HDR
done

python3 "$here/tools/fd2stub.py" "$work/stubs" "$NDK"/fd/*_lib.fd
for s in "$work"/stubs/*.s; do
  $cc -c "$s" -o "${s%.s}.o"
done
$cc -c "$here/crt/crt0.s" -o "$out/lib/crt0.o"
$cc -c "$here/crt/amiga_main.c" -o "$work/amiga_main.o"
# memcpy and friends, until picolibc has an AmigaOS layer.
$cc -ffreestanding -fno-builtin -c "$here/crt/minilibc.c" -o "$work/minilibc.o"
rm -f "$out/lib/libc.a"
"${bin}llvm-ar" rcs "$out/lib/libc.a" "$work/minilibc.o"
rm -f "$out/lib/libamiga.a"
"${bin}llvm-ar" rcs "$out/lib/libamiga.a" "$work"/amiga_main.o "$work"/stubs/*.o
echo "sysroot written to $out"
