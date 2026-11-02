// Exercises the AmigaOS ABI pieces: NDK headers, OS calls through stubs and
// through register-variable inline asm (which loads d2/d3/a6), locals that
// live across those calls, globals in data and bss hunks, struct return and
// pointer returns.
#include <exec/types.h>
#include <exec/execbase.h>
#include <dos/dos.h>
#include <proto/exec.h>
#include <proto/dos.h>

static inline LONG InlineWrite(BPTR fh, APTR buf, LONG len) {
  register LONG _r __asm("d0");
  register struct Library *_b __asm("a6") = DOSBase;
  register BPTR _d1 __asm("d1") = fh;
  register APTR _d2 __asm("d2") = buf;
  register LONG _d3 __asm("d3") = len;
  __asm volatile("jsr (-48,%%a6)"
                 : "=r"(_r), "+r"(_d1)
                 : "r"(_b), "r"(_d2), "r"(_d3)
                 : "a0", "a1", "cc", "memory");
  return _r;
}

static long slen(const char *s) {
  long n = 0;
  while (s[n])
    ++n;
  return n;
}

static void say(const char *s) { InlineWrite(Output(), (APTR)s, slen(s)); }

static void hex(unsigned long v) {
  char b[11];
  b[0] = '$';
  for (int i = 0; i < 8; ++i)
    b[1 + i] = "0123456789ABCDEF"[(v >> (28 - 4 * i)) & 15];
  b[9] = '\n';
  b[10] = 0;
  say(b);
}

struct Big { long a, b, c, d; };
static struct Big __attribute__((noinline)) makeBig(long x) {
  struct Big r = {x, x * 2, x * 3, x * 4};
  return r;
}

int counter = 5;       // data hunk
static char buffer[64]; // bss hunk
const char *names[] = {"zero", "one", "two"}; // data with relocations

int main(int argc, char **argv) {
  long locals[4] = {11, 22, 33, 44};
  int ok = 1;
  say("ABI test\n");
  for (int i = 0; i < 3; ++i) {
    say(names[i]);
    say("\n");
    // The inline call sets d2, d3 and a6; the frame (a5) and these locals
    // must survive it.
    if (locals[i] != 11 * (i + 1))
      ok = 0;
  }
  struct Big b = makeBig(7);
  if (b.a != 7 || b.b != 14 || b.c != 21 || b.d != 28)
    ok = 0;
  for (int i = 0; i < 63; ++i)
    buffer[i] = 'a' + i % 26;
  if (buffer[30] != 'e')
    ok = 0;
  counter += argc;
  struct Library *lib = OpenLibrary((STRPTR) "dos.library", 0);
  if (lib != DOSBase)
    ok = 0;
  CloseLibrary(lib);
  say("SysBase version: ");
  hex(SysBase->LibNode.lib_Version);
  say(ok ? "PASS\n" : "FAIL\n");
  return ok ? 0 : 10;
}
