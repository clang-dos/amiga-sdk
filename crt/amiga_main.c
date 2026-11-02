// C side of the AmigaOS startup code: open dos.library, split the CLI
// argument string into argv, call main(), and clean up in exit().

#include <stddef.h>

struct Library;
extern struct Library *OpenLibrary(const char *name, unsigned long version);
extern void CloseLibrary(struct Library *lib);
extern void *AllocMem(unsigned long size, unsigned long flags);
extern void FreeMem(void *mem, unsigned long size);

struct Library *DOSBase;
extern void *_WBenchMsg;
extern char *__amiga_argstr;
extern long __amiga_arglen;

extern int main(int argc, char **argv);
extern void _exit(int status) __attribute__((noreturn));
void exit(int status) __attribute__((noreturn));

#define MAX_ATEXIT 32
static void (*atexit_fns[MAX_ATEXIT])(void);
static int atexit_count;

int atexit(void (*fn)(void)) {
  if (atexit_count == MAX_ATEXIT)
    return -1;
  atexit_fns[atexit_count++] = fn;
  return 0;
}

static char *argbuf;
static unsigned long argbuf_size;

void exit(int status) {
  while (atexit_count > 0)
    atexit_fns[--atexit_count]();
  if (argbuf)
    FreeMem(argbuf, argbuf_size);
  if (DOSBase)
    CloseLibrary(DOSBase);
  _exit(status);
}

// Split "prog args..." the way the CLI passes it: words separated by
// spaces, double quotes group words, *" and *N escape inside quotes.
static int split_args(char **argv, char *out, const char *s, long n) {
  int argc = 1;
  long i = 0;
  while (i < n) {
    while (i < n && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n'))
      ++i;
    if (i >= n)
      break;
    if (argv)
      argv[argc] = out;
    if (s[i] == '"') {
      ++i;
      while (i < n && s[i] != '"' && s[i] != '\n') {
        char c = s[i++];
        if (c == '*' && i < n) {
          c = s[i++];
          if (c == 'n' || c == 'N')
            c = '\n';
          else if (c == 'e' || c == 'E')
            c = 0x1b;
        }
        if (out && argv)
          *out++ = c;
      }
      ++i;
    } else {
      while (i < n && s[i] != ' ' && s[i] != '\t' && s[i] != '\n') {
        if (out && argv)
          *out++ = s[i];
        ++i;
      }
    }
    if (out && argv)
      *out++ = 0;
    ++argc;
  }
  return argc;
}

int __amiga_main(void) {
  DOSBase = OpenLibrary("dos.library", 0);
  if (!DOSBase)
    return 20;
  if (_WBenchMsg)
    exit(main(0, (char **)_WBenchMsg));

  int argc = split_args(NULL, NULL, __amiga_argstr, __amiga_arglen);
  argbuf_size = (argc + 1) * sizeof(char *) + __amiga_arglen + 2;
  argbuf = AllocMem(argbuf_size, 1 /* MEMF_PUBLIC */);
  if (!argbuf)
    exit(20);
  char **argv = (char **)argbuf;
  char *strings = argbuf + (argc + 1) * sizeof(char *);
  split_args(argv, strings, __amiga_argstr, __amiga_arglen);
  argv[0] = "";
  argv[argc] = NULL;
  exit(main(argc, argv));
}
