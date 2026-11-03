#!/bin/sh
# Build the m68k-amigaos sysroot: picolibc with its AmigaOS layer, the
# compiler-rt builtins (both for the 68000, so they run on every 68k Amiga),
# and the AmigaOS headers and libamiga.a, built from AROS.
#
#   BIN=... LLVM_SRC=... PICOLIBC_SRC=... ./build-sysroot.sh OUT
#
#   BIN           directory with clang, llvm-ar and the other LLVM tools
#   LLVM_SRC      clang-dos/llvm checkout (compiler-rt)
#   PICOLIBC_SRC  clang-dos/picolibc checkout
#   WORK          build directory (default: ./build-amigaos)
#   AROS_SRC      AROS checkout (optional; by default tools/fetch-aros.sh
#                 fetches the headers of the tested commit into WORK)
#   NDK           NDK3.2.lha (optional): install Hyperion's NDK headers in
#                 place of the SDK's, as tools/amiga-ndk.py does
#
# OUT gets include/ (the C library and AmigaOS headers), fd/, lib/crt0.o,
# lib/libc.a, lib/libm.a, lib/libamiga.a and lib/libclang_rt.builtins.a.
# Everything in it can be redistributed.
set -e
out=${1:?usage: build-sysroot.sh OUT}
: "${BIN:?set BIN}" "${LLVM_SRC:?set LLVM_SRC}" "${PICOLIBC_SRC:?set PICOLIBC_SRC}"
here=$(cd "$(dirname "$0")" && pwd)
WORK=${WORK:-$PWD/build-amigaos}
mkdir -p "$out/include" "$out/lib" "$WORK"
out=$(cd "$out" && pwd) WORK=$(cd "$WORK" && pwd)
PATH=$BIN:$PATH
export PATH

# picolibc. Its malloc is replaced by the AmigaOS layer's (AllocMem).
rm -rf "$WORK/picolibc" "$WORK/picolibc-install"
meson setup "$WORK/picolibc" "$PICOLIBC_SRC" \
  --cross-file "$PICOLIBC_SRC/scripts/cross-clang-m68k-amigaos.txt" \
  -Dos-amigaos=true -Denable-malloc=false -Dmultilib=false -Dpicocrt=false \
  -Dsemihost=false -Dtests=false -Dposix-console=true -Dsingle-thread=true \
  -Dthread-local-storage=false -Dspecsdir=none \
  --prefix="$WORK/picolibc-install" >"$WORK/picolibc.log" 2>&1 ||
  { tail -20 "$WORK/picolibc.log"; exit 1; }
ninja -C "$WORK/picolibc" install >>"$WORK/picolibc.log" 2>&1 ||
  { grep -E 'error|FAILED' "$WORK/picolibc.log" | head -20; exit 1; }
cp -R "$WORK/picolibc-install/include/." "$out/include/"
for f in crt0.o libc.a libm.a; do
  cp "$WORK/picolibc-install/lib/$f" "$out/lib/"
done

# compiler-rt builtins: 32-bit multiply and divide (the 68000 has only
# 16-bit ones), 64-bit arithmetic and soft float.
cmake -G Ninja -S "$LLVM_SRC/compiler-rt/lib/builtins" -B "$WORK/builtins" \
  -DCMAKE_SYSTEM_NAME=Generic -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER="$BIN/clang" -DCMAKE_ASM_COMPILER="$BIN/clang" \
  -DCMAKE_CXX_COMPILER="$BIN/clang++" \
  -DCMAKE_C_COMPILER_TARGET=m68k-unknown-amigaos \
  -DCMAKE_CXX_COMPILER_TARGET=m68k-unknown-amigaos \
  -DCMAKE_ASM_COMPILER_TARGET=m68k-unknown-amigaos \
  -DCMAKE_C_FLAGS="-mcpu=68000" -DCMAKE_ASM_FLAGS="-mcpu=68000" \
  -DCMAKE_AR="$BIN/llvm-ar" -DCMAKE_RANLIB="$BIN/llvm-ranlib" \
  -DCMAKE_NM="$BIN/llvm-nm" \
  -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY \
  -DCOMPILER_RT_DEFAULT_TARGET_ONLY=ON -DCOMPILER_RT_BAREMETAL_BUILD=ON \
  -DCOMPILER_RT_OS_DIR=amigaos -DLLVM_CMAKE_DIR="$BIN/../lib/cmake/llvm" \
  >"$WORK/builtins.log" 2>&1 ||
  { tail -20 "$WORK/builtins.log"; exit 1; }
ninja -C "$WORK/builtins" >>"$WORK/builtins.log" 2>&1 ||
  { grep -E 'error|FAILED' "$WORK/builtins.log" | head -20; exit 1; }
cp "$WORK/builtins/lib/amigaos/libclang_rt.builtins-m68k.a" \
   "$out/lib/libclang_rt.builtins.a"

# The AmigaOS headers, fd files and libamiga.a (OS call stubs and the
# amiga.lib helpers). No file name is in both these and the C library's.
if [ -z "$AROS_SRC" ]; then
  AROS_SRC=$WORK/aros
  "$here/tools/fetch-aros.sh" "$AROS_SRC"
fi
rm -rf "$WORK/os"
python3 "$here/tools/aros-ndk.py" --aros "$AROS_SRC" --out "$WORK/os" --bin "$BIN"
cp -R "$WORK/os/include/." "$out/include/"
mkdir -p "$out/fd"
cp -R "$WORK/os/fd/." "$out/fd/"
cp "$WORK/os/lib/libamiga.a" "$out/lib/"

if [ -n "$NDK" ]; then
  python3 "$here/tools/amiga-ndk.py" --sysroot "$out" --archive "$NDK" --bin "$BIN"
fi
echo "sysroot written to $out"
