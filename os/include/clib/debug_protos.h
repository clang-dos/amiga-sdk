#ifndef CLIB_DEBUG_PROTOS_H
#define CLIB_DEBUG_PROTOS_H

/*
 * debug.lib: debug output on the serial port (in libamiga.a). Output and
 * input go through exec's raw I/O routines, so they work with interrupts
 * off and before dos.library is up. The format strings are RawDoFmt ones:
 * %d reads a WORD, %ld a LONG.
 *
 * Part of the Amiga SDK headers. See os/README.md.
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

LONG KCmpStr(CONST_STRPTR string1, CONST_STRPTR string2);
VOID KDoFmt(CONST_STRPTR formatString, CONST APTR dataStream, CONST APTR putChProc, APTR putChData);
LONG KGetChar(VOID);
LONG KGetNum(VOID);
LONG KMayGetChar(VOID);
VOID KPrintF(CONST_STRPTR formatString, ...);
VOID KPutChar(LONG ch);
VOID KPutFmt(CONST_STRPTR formatString, CONST APTR values);
VOID KPutStr(CONST_STRPTR string);
VOID KVPrintF(CONST_STRPTR formatString, CONST APTR values);

/* Other names for the same functions. */
LONG KGetCh(VOID);
LONG KMayGetCh(VOID);
VOID KPutCh(LONG ch);
VOID KPutS(CONST_STRPTR string);
LONG cmpstrexec(CONST_STRPTR string1, CONST_STRPTR string2);
LONG kgetc(VOID);
LONG kgetch(VOID);
LONG kgetchar(VOID);
LONG kgetnum(VOID);
VOID kprintf(CONST_STRPTR formatString, ...);
VOID kputc(LONG ch);
VOID kputch(LONG ch);
VOID kputchar(LONG ch);
VOID kputs(CONST_STRPTR string);

#ifdef __cplusplus
}
#endif

#endif /* CLIB_DEBUG_PROTOS_H */
