// ddebug.lib: debug output on the parallel port.
//
// Part of the Amiga SDK (see os/README.md). The characters go straight to
// the port hardware: CIA-A port B holds the data, and writing it strobes
// the printer; CIA-B port A bit 0 is the printer's BUSY line. The port
// has no input, so DMayGetChar always returns -1.

        .text

CIAA_PRB  = 0xbfe101
CIAA_DDRB = 0xbfe301
CIAB_PRA  = 0xbfd000

// VOID DPutChar(LONG ch)
        .globl  DPutChar, DPutCh, dputc, dputch, dputchar
DPutChar:
DPutCh:
dputc:
dputch:
dputchar:
        move.l  (4,%sp),%d0
putc:   move.l  #0x10000,%d1    // do not wait for ever if nothing is plugged in
        lea     CIAB_PRA,%a1
1:      btst    #0,(%a1)
        beq     2f
        sub.l   #1,%d1
        bne     1b
2:      lea     CIAA_DDRB,%a1
        moveq   #-1,%d1
        move.b  %d1,(%a1)
        lea     CIAA_PRB,%a1
        move.b  %d0,(%a1)
        rts

// LONG DMayGetChar(VOID)
        .globl  DMayGetChar, DMayGetCh
DMayGetChar:
DMayGetCh:
        moveq   #-1,%d0
        rts

// LONG DGetChar(VOID): the port has no input, so this never returns
        .globl  DGetChar, DGetCh, dgetc, dgetch, dgetchar
DGetChar:
DGetCh:
dgetc:
dgetch:
dgetchar:
1:      bsr.w   DMayGetChar
        cmpi.l  #0,%d0
        bmi     1b
        rts

// VOID DPutStr(CONST_STRPTR string)
        .globl  DPutStr, DPutS, dputs, dputstr
DPutStr:
DPutS:
dputs:
dputstr:
        move.l  (4,%sp),%a0
1:      moveq   #0,%d0
        move.b  (%a0)+,%d0
        beq     2f
        bsr.w   putc
        bra     1b
2:      rts

// VOID DDoFmt(fmt, dataStream, putChProc, putChData): RawDoFmt
        .globl  DDoFmt
DDoFmt:
        movem.l %a2-%a3/%a6,-(%sp)
        movem.l (16,%sp),%a0-%a3
        move.l  4,%a6
        jsr     (-522,%a6)
        movem.l (%sp)+,%a2-%a3/%a6
        rts

putproc:
        cmpi.b  #0,%d0
        bne     putc
        rts

// VOID DPutFmt(CONST_STRPTR fmt, CONST APTR values)
        .globl  DPutFmt
DPutFmt:
        move.l  (8,%sp),%a1
        bra     fmt

// VOID DPrintF(CONST_STRPTR fmt, ...)
        .globl  DPrintF, dprintf
DPrintF:
dprintf:
        lea     (8,%sp),%a1
fmt:    movem.l %a2-%a3/%a6,-(%sp)
        move.l  (16,%sp),%a0
        lea     putproc(%pc),%a2
        move.l  4,%a6
        jsr     (-522,%a6)
        movem.l (%sp)+,%a2-%a3/%a6
        rts
