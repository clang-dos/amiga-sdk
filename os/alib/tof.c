/*
 * amiga.lib: vertical blank helpers (AddTOF, RemTOF).
 * Part of the Amiga SDK; see os/README.md.
 */
#include <exec/types.h>
#include <exec/interrupts.h>
#include <hardware/intbits.h>
#include <graphics/graphint.h>
#include <proto/exec.h>
#include <clib/alib_protos.h>

/* The interrupt server: exec calls it with is_Data (the Isrvstr) in a1.
 * It calls ccode(Carg) and returns 0, so the rest of the chain runs. */
extern LONG __tof_server(void);
__asm__(
    "        .text\n"
    "        .globl  __tof_server\n"
    "__tof_server:\n"
    "        move.l  (26,%a1),-(%sp)\n"   /* Carg */
    "        move.l  (22,%a1),%a0\n"      /* ccode */
    "        jsr     (%a0)\n"
    "        lea     (4,%sp),%sp\n"
    "        moveq   #0,%d0\n"
    "        rts\n");

VOID AddTOF(struct Isrvstr *i, LONG (*p)(APTR args), APTR a)
{
    i->is_Node.ln_Type = NT_INTERRUPT;
    i->is_Node.ln_Pri = 0;
    i->Iptr = i;
    i->code = __tof_server;
    i->ccode = p;
    i->Carg = a;
    AddIntServer(INTB_VERTB, (struct Interrupt *)i);
}

VOID RemTOF(struct Isrvstr *i)
{
    RemIntServer(INTB_VERTB, (struct Interrupt *)i);
}
