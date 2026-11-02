// Prints a greeting through dos.library, and its arguments.
struct Library;
extern long Output(void);
extern long Write(long file, const void *buf, long len);

static long len(const char *s) {
  long n = 0;
  while (s[n])
    ++n;
  return n;
}

static void put(const char *s) { Write(Output(), s, len(s)); }

int main(int argc, char **argv) {
  put("Hello from clang on AmigaOS!\n");
  for (int i = 1; i < argc; ++i) {
    put("arg: ");
    put(argv[i]);
    put("\n");
  }
  return 0;
}
