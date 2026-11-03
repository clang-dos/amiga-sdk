/*
 * reaction.lib: LibDoGadgetMethodA and the listbrowser node methods.
 * Part of the Amiga SDK; see os/README.md.
 */
#include <exec/types.h>
#include <intuition/intuitionbase.h>
#include <gadgets/listbrowser.h>
#include <proto/intuition.h>
#include <clib/alib_protos.h>
#include <clib/reaction_lib_protos.h>

/* DoGadgetMethodA() on V39 and later. Intuition V37 has none, and without
 * a window there is no GadgetInfo to make, so then the method goes
 * straight to the gadget, without one. */
ULONG LibDoGadgetMethodA(struct Gadget *gad, struct Window *win, struct Requester *req, Msg msg)
{
    if (win && IntuitionBase->LibNode.lib_Version >= 39)
        return DoGadgetMethodA(gad, win, req, msg);
    return DoMethodA((Object *)gad, msg);
}

struct Node *LBAddNodeA(struct Gadget *lb, struct Window *w, struct Requester *r,
                        struct Node *n, CONST struct TagItem *tags)
{
    struct lbAddNode m = { LBM_ADDNODE, NULL, n, (struct TagItem *)tags };
    return (struct Node *)LibDoGadgetMethodA(lb, w, r, (Msg)&m);
}

ULONG LBEditNodeA(struct Gadget *lb, struct Window *w, struct Requester *r,
                  struct Node *n, CONST struct TagItem *tags)
{
    struct lbEditNode m = { LBM_EDITNODE, NULL, n, (struct TagItem *)tags };
    return LibDoGadgetMethodA(lb, w, r, (Msg)&m);
}

ULONG LBRemNode(struct Gadget *lb, struct Window *w, struct Requester *r, struct Node *n)
{
    struct lbRemNode m = { LBM_REMNODE, NULL, n };
    return LibDoGadgetMethodA(lb, w, r, (Msg)&m);
}

/* The varargs forms: the tag list (or the message) is the rest of the
 * arguments on the stack. Each "move.l (N,%sp),-(%sp)" copies the next
 * fixed argument down, since every push moves the stack by 4. */
__asm__(
    "        .text\n"
    "        .globl  LBAddNode, LBEditNode, LibDoGadgetMethod\n"
    "LBAddNode:\n"
    "        lea     (20,%sp),%a0\n"
    "        move.l  %a0,-(%sp)\n"
    "        move.l  (20,%sp),-(%sp)\n"
    "        move.l  (20,%sp),-(%sp)\n"
    "        move.l  (20,%sp),-(%sp)\n"
    "        move.l  (20,%sp),-(%sp)\n"
    "        jsr     LBAddNodeA\n"
    "        lea     (20,%sp),%sp\n"
    "        rts\n"
    "LBEditNode:\n"
    "        lea     (20,%sp),%a0\n"
    "        move.l  %a0,-(%sp)\n"
    "        move.l  (20,%sp),-(%sp)\n"
    "        move.l  (20,%sp),-(%sp)\n"
    "        move.l  (20,%sp),-(%sp)\n"
    "        move.l  (20,%sp),-(%sp)\n"
    "        jsr     LBEditNodeA\n"
    "        lea     (20,%sp),%sp\n"
    "        rts\n"
    "LibDoGadgetMethod:\n"
    "        lea     (16,%sp),%a0\n"
    "        move.l  %a0,-(%sp)\n"
    "        move.l  (16,%sp),-(%sp)\n"
    "        move.l  (16,%sp),-(%sp)\n"
    "        move.l  (16,%sp),-(%sp)\n"
    "        jsr     LibDoGadgetMethodA\n"
    "        lea     (16,%sp),%sp\n"
    "        rts\n");
