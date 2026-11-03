// debug.lib: debug output on the serial port, through exec's raw I/O.
//
// Part of the Amiga SDK (see os/README.md). The raw I/O entry points are
// exec's own (RawMayGetChar -510, RawPutChar -516, RawDoFmt -522), present
// in every Kickstart from 1.2 on. Exec sets the serial port to 9600 baud.

        .text

// VOID KPutChar(LONG ch)
        .globl  KPutChar, KPutCh, kputc, kputch, kputchar
KPutChar:
KPutCh:
kputc:
kputch:
kputchar:
        move.l  (4,%sp),%d0
putc:   move.l  %a6,-(%sp)
        move.l  4,%a6
        jsr     (-516,%a6)
        move.l  (%sp)+,%a6
        rts

// LONG KMayGetChar(VOID): the next character, or -1 if none is waiting
        .globl  KMayGetChar, KMayGetCh
KMayGetChar:
KMayGetCh:
        move.l  %a6,-(%sp)
        move.l  4,%a6
        jsr     (-510,%a6)
        move.l  (%sp)+,%a6
        rts

// LONG KGetChar(VOID): waits for a character
        .globl  KGetChar, KGetCh, kgetc, kgetch, kgetchar
KGetChar:
KGetCh:
kgetc:
kgetch:
kgetchar:
1:      bsr.w   KMayGetChar
        cmpi.l  #0,%d0
        bmi     1b
        rts

// VOID KPutStr(CONST_STRPTR string)
        .globl  KPutStr, KPutS, kputs
KPutStr:
KPutS:
kputs:
        move.l  %a2,-(%sp)
        move.l  (8,%sp),%a2
1:      moveq   #0,%d0
        move.b  (%a2)+,%d0
        beq     2f
        bsr.w   putc
        bra     1b
2:      move.l  (%sp)+,%a2
        rts

// VOID KDoFmt(fmt, dataStream, putChProc, putChData): RawDoFmt
        .globl  KDoFmt
KDoFmt:
        movem.l %a2-%a3/%a6,-(%sp)
        movem.l (16,%sp),%a0-%a3
        move.l  4,%a6
        jsr     (-522,%a6)
        movem.l (%sp)+,%a2-%a3/%a6
        rts

// RawDoFmt's PutChProc: the character in d0, a3 free. The final NUL is
// not sent.
putproc:
        cmpi.b  #0,%d0
        beq     1f
        bra     putc
1:      rts

// VOID KPutFmt(CONST_STRPTR fmt, CONST APTR values)
// VOID KVPrintF(CONST_STRPTR fmt, CONST APTR values)
        .globl  KPutFmt, KVPrintF
KPutFmt:
KVPrintF:
        move.l  (8,%sp),%a1
        bra     fmt

// VOID KPrintF(CONST_STRPTR fmt, ...): the values follow on the stack
        .globl  KPrintF, kprintf
KPrintF:
kprintf:
        lea     (8,%sp),%a1
fmt:    movem.l %a2-%a3/%a6,-(%sp)
        move.l  (16,%sp),%a0
        lea     putproc(%pc),%a2
        move.l  4,%a6
        jsr     (-522,%a6)
        movem.l (%sp)+,%a2-%a3/%a6
        rts
