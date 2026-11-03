/*
 * reaction.lib: ClickTabsA, ClickTabs and FreeClickTabs (needs the clicktab class open).
 * Part of the Amiga SDK; see os/README.md.
 */
#include <exec/types.h>
#include <exec/memory.h>
#include <gadgets/clicktab.h>
#include <proto/exec.h>
#include <proto/clicktab.h>
#include <clib/alib_protos.h>
#include <clib/reaction_lib_protos.h>

VOID FreeClickTabs(struct List *list)
{
    struct Node *n;

    if (!list)
        return;
    while ((n = RemHead(list)))
        FreeClickTabNode(n);
    FreeMem(list, sizeof(*list));
}

/* One node for each label of the NULL-terminated array, numbered from 0. */
struct List *ClickTabsA(STRPTR *labels)
{
    struct List *list = AllocMem(sizeof(*list), MEMF_ANY);
    struct Node *n;
    LONG i;

    if (!list)
        return NULL;
    NewList(list);
    for (i = 0; labels[i]; i++) {
        if (!(n = AllocClickTabNode(TNA_Text, (ULONG)labels[i], TNA_Number, i, TAG_DONE))) {
            FreeClickTabs(list);
            return NULL;
        }
        AddTail(list, n);
    }
    return list;
}

/* The labels follow label1 on the stack. */
__asm__(
    "        .text\n"
    "        .globl  ClickTabs\n"
    "ClickTabs:\n"
    "        lea     (4,%sp),%a0\n"
    "        move.l  %a0,-(%sp)\n"
    "        jsr     ClickTabsA\n"
    "        lea     (4,%sp),%sp\n"
    "        rts\n");
