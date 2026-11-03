/*
 * reaction.lib: RadioButtonsA, RadioButtons and FreeRadioButtons (needs the radiobutton class open).
 * Part of the Amiga SDK; see os/README.md.
 */
#include <exec/types.h>
#include <exec/memory.h>
#include <gadgets/radiobutton.h>
#include <proto/exec.h>
#include <proto/radiobutton.h>
#include <clib/alib_protos.h>
#include <clib/reaction_lib_protos.h>

VOID FreeRadioButtons(struct List *list)
{
    struct Node *n;

    if (!list)
        return;
    while ((n = RemHead(list)))
        FreeRadioButtonNode(n);
    FreeMem(list, sizeof(*list));
}

/* One node for each label of the NULL-terminated array, numbered from 0. */
struct List *RadioButtonsA(STRPTR *labels)
{
    struct List *list = AllocMem(sizeof(*list), MEMF_ANY);
    struct Node *n;
    LONG i;

    if (!list)
        return NULL;
    NewList(list);
    for (i = 0; labels[i]; i++) {
        if (!(n = AllocRadioButtonNode(0, RBNA_Label, (ULONG)labels[i], TAG_DONE))) {
            FreeRadioButtons(list);
            return NULL;
        }
        AddTail(list, n);
    }
    return list;
}

/* The labels follow label1 on the stack. */
__asm__(
    "        .text\n"
    "        .globl  RadioButtons\n"
    "RadioButtons:\n"
    "        lea     (4,%sp),%a0\n"
    "        move.l  %a0,-(%sp)\n"
    "        jsr     RadioButtonsA\n"
    "        lea     (4,%sp),%sp\n"
    "        rts\n");
