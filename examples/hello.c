// Prints a greeting with the C library, and its arguments, and the
// Kickstart version from exec.library.
#include <stdio.h>

#include <exec/execbase.h>
#include <proto/exec.h>

int main(int argc, char **argv) {
  printf("Hello from clang on AmigaOS %d.%d!\n", SysBase->LibNode.lib_Version,
         SysBase->SoftVer);
  for (int i = 1; i < argc; ++i)
    printf("arg %d: %s\n", i, argv[i]);
  return 0;
}
