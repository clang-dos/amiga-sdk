/*
 * reaction.lib: BrowserNodesA, BrowserNodes and FreeBrowserNodes (needs the listbrowser class open).
 * Part of the Amiga SDK; see os/README.md.
 */
#include <exec/types.h>
#include <exec/memory.h>
#include <gadgets/listbrowser.h>
#include <proto/exec.h>
#include <proto/listbrowser.h>
#include <clib/alib_protos.h>
#include <clib/reaction_lib_protos.h>

VOID FreeBrowserNodes(struct List *list)
{
    struct Node *n;

    if (!list)
        return;
    while ((n = RemHead(list)))
        FreeListBrowserNode(n);
    FreeMem(list, sizeof(*list));
}

/* One node for each label of the NULL-terminated array, numbered from 0. */
struct List *BrowserNodesA(STRPTR *labels)
{
    struct List *list = AllocMem(sizeof(*list), MEMF_ANY);
    struct Node *n;
    LONG i;

    if (!list)
        return NULL;
    NewList(list);
    for (i = 0; labels[i]; i++) {
        if (!(n = AllocListBrowserNode(1, LBNCA_Text, (ULONG)labels[i], TAG_DONE))) {
            FreeBrowserNodes(list);
            return NULL;
        }
        AddTail(list, n);
    }
    return list;
}

/* The labels follow label1 on the stack. */
__asm__(
    "        .text\n"
    "        .globl  BrowserNodes\n"
    "BrowserNodes:\n"
    "        lea     (4,%sp),%a0\n"
    "        move.l  %a0,-(%sp)\n"
    "        jsr     BrowserNodesA\n"
    "        lea     (4,%sp),%sp\n"
    "        rts\n");
