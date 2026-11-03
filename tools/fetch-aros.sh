#!/bin/sh
# Fetch the AROS sources the SDK's AmigaOS headers are built from: only the
# header files and the makefiles that say where they are installed, at the
# commit the SDK is tested with.
#
#   tools/fetch-aros.sh DIR
set -e
AROS_URL=${AROS_URL:-https://github.com/aros-development-team/AROS}
AROS_COMMIT=${AROS_COMMIT:-128dae41825d09c1a877ea4e20d959a823af9563}
dir=${1:?usage: fetch-aros.sh DIR}

paths() {
  git sparse-checkout set --no-cone '/LICENSE' '/ACKNOWLEDGEMENTS' \
  '/compiler/include/' '/compiler/arossupport/include/' \
  '/arch/m68k-all/**/*.h' '/arch/m68k-all/**/mmakefile.src' \
  '/arch/m68k-amiga/**/*.h' '/arch/m68k-amiga/**/mmakefile.src' \
  '/compiler/include/exec/execbase.inc' \
  '/rom/**/*.h' '/rom/**/mmakefile.src' \
  '/workbench/libs/**/*.h' '/workbench/libs/**/mmakefile.src' \
  '/workbench/devs/**/*.h' '/workbench/devs/**/mmakefile.src' \
  '/workbench/classes/**/*.h' '/workbench/classes/**/mmakefile.src' \
  '/workbench/c/**/*.h' '/workbench/c/**/mmakefile.src'
}

# Already there: only make sure the list of paths is the current one.
if [ "$(git -C "$dir" rev-parse HEAD 2>/dev/null)" = "$AROS_COMMIT" ]; then
  cd "$dir"
  paths
  exit 0
fi
rm -rf "$dir"
git init -q "$dir"
cd "$dir"
git remote add origin "$AROS_URL"
paths
git fetch -q --depth 1 --filter=blob:none origin "$AROS_COMMIT"
git checkout -q FETCH_HEAD
