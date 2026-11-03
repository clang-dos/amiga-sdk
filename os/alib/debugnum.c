/*
 * debug.lib and ddebug.lib: reading a number, comparing strings.
 * Part of the Amiga SDK; see os/README.md.
 */
#include <exec/types.h>
#include <clib/debug_protos.h>
#include <clib/ddebug_protos.h>

/* Reads a decimal number, echoing it: an optional '-', digits, then Return.
 * Backspace deletes the last digit. */
static LONG getnum(LONG (*get)(VOID), VOID (*put)(LONG))
{
    LONG n = 0, neg = 0, digits = 0, c;

    for (;;) {
        c = get();
        if (c == '\r' || c == '\n') {
            put('\r');
            put('\n');
            return neg ? -n : n;
        }
        if (c == '-' && !digits && !neg) {
            neg = 1;
        } else if (c >= '0' && c <= '9') {
            n = n * 10 + (c - '0');
            digits++;
        } else if ((c == 8 || c == 127) && (digits || neg)) {
            if (digits) {
                n /= 10;
                digits--;
            } else {
                neg = 0;
            }
            put(8);
            put(' ');
        } else {
            continue;
        }
        put(c == 127 ? 8 : c);
    }
}

LONG KGetNum(VOID) { return getnum(KGetChar, KPutChar); }
LONG kgetnum(VOID) __attribute__((alias("KGetNum")));
LONG DGetNum(VOID) { return getnum(DGetChar, DPutChar); }
LONG dgetnum(VOID) __attribute__((alias("DGetNum")));

/* Like strcmp: less than, equal to or greater than 0. */
LONG KCmpStr(CONST_STRPTR s1, CONST_STRPTR s2)
{
    while (*s1 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (LONG)*(const UBYTE *)s1 - (LONG)*(const UBYTE *)s2;
}
LONG cmpstrexec(CONST_STRPTR s1, CONST_STRPTR s2) __attribute__((alias("KCmpStr")));
