#ifndef CLIB_DDEBUG_PROTOS_H
#define CLIB_DDEBUG_PROTOS_H

/*
 * ddebug.lib: debug output on the parallel port (in libamiga.a). The
 * functions drive the port hardware directly, so nothing else may use the
 * port at the same time. The parallel port has no input: DMayGetChar
 * returns -1 and DGetChar and DGetNum wait forever.
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
VOID DDoFmt(CONST_STRPTR formatString, CONST APTR dataStream, CONST APTR putChProc, APTR putChData);
LONG DGetChar(VOID);
LONG DGetNum(VOID);
LONG DMayGetChar(VOID);
VOID DPrintF(CONST_STRPTR formatString, ...);
VOID DPutChar(LONG ch);
VOID DPutFmt(CONST_STRPTR formatString, CONST APTR values);
VOID DPutStr(CONST_STRPTR string);

/* Other names for the same functions. */
LONG DGetCh(VOID);
LONG DMayGetCh(VOID);
VOID DPutCh(LONG ch);
VOID DPutS(CONST_STRPTR string);
LONG cmpstrexec(CONST_STRPTR string1, CONST_STRPTR string2);
LONG dgetc(VOID);
LONG dgetch(VOID);
LONG dgetchar(VOID);
LONG dgetnum(VOID);
VOID dprintf(CONST_STRPTR formatString, ...);
VOID dputc(LONG ch);
VOID dputch(LONG ch);
VOID dputchar(LONG ch);
VOID dputs(CONST_STRPTR string);
VOID dputstr(CONST_STRPTR string);

#ifdef __cplusplus
}
#endif

#endif /* CLIB_DDEBUG_PROTOS_H */
