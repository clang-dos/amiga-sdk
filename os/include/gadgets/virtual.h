#ifndef GADGETS_VIRTUAL_H
#define GADGETS_VIRTUAL_H
/*
    virtual.gadget: a scrollable view onto a larger layout group.
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef REACTION_REACTION_H
#include <reaction/reaction.h>
#endif
#ifndef INTUITION_GADGETCLASS_H
#include <intuition/gadgetclass.h>
#endif

#define VIRTUALA_Base           (REACTION_Dummy + 0x24500)
#define VIRTUALA_Contents       (VIRTUALA_Base + 0)     /* (Object *) the group to show */
#define VIRTUALA_Scroller       (VIRTUALA_Base + 1)     /* (BOOL) add scrollers */
#define VIRTUALA_ScrollX        (VIRTUALA_Base + 2)     /* (LONG) scroll by this much */
#define VIRTUALA_ScrollY        (VIRTUALA_Base + 3)
#define VIRTUALA_VisibleX       (VIRTUALA_Base + 4)     /* (LONG) visible size (get) */
#define VIRTUALA_VisibleY       (VIRTUALA_Base + 5)
#define VIRTUALA_TopX           (VIRTUALA_Base + 6)     /* (LONG) scroll position */
#define VIRTUALA_TopY           (VIRTUALA_Base + 7)
#define VIRTUALA_TotalX         (VIRTUALA_Base + 8)     /* (LONG) size of the contents (get) */
#define VIRTUALA_TotalY         (VIRTUALA_Base + 9)
#define VIRTUALA_ScrollerX      (VIRTUALA_Base + 10)    /* (Object *) horizontal scroller */
#define VIRTUALA_ScrollerY      (VIRTUALA_Base + 11)    /* (Object *) vertical scroller */
#define VIRTUALA_InputScroll    (VIRTUALA_Base + 12)    /* (BOOL) scroll with the keyboard and wheel */
#define VIRTUALA_NoDispose      (VIRTUALA_Base + 13)    /* (BOOL) keep the contents on dispose */
#define VIRTUALA_TargetX        (VIRTUALA_Base + 14)    /* (LONG) make this point visible */
#define VIRTUALA_TargetY        (VIRTUALA_Base + 15)
#define VIRTUALA_MapX           (VIRTUALA_Base + 16)    /* (LONG) map coordinates (get) */
#define VIRTUALA_MapY           (VIRTUALA_Base + 17)

#define VirtualObject   NewObject(VIRTUAL_GetClass(), NULL
#define VirtualEnd      TAG_END)

#endif /* GADGETS_VIRTUAL_H */
