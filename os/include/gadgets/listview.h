#ifndef GADGETS_LISTVIEW_H
#define GADGETS_LISTVIEW_H
/*
    listview.gadget: a simple list of text lines.
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef EXEC_NODES_H
#include <exec/nodes.h>
#endif
#ifndef INTUITION_INTUITION_H
#include <intuition/intuition.h>
#endif
#ifndef INTUITION_GADGETCLASS_H
#include <intuition/gadgetclass.h>
#endif

#define LISTVIEW_Dummy          (TAG_USER + 0x04000000)
#define LISTVIEW_Labels         (LISTVIEW_Dummy + 1)   /* (struct List *) of ListLabelNodes */
#define LISTVIEW_Top            (LISTVIEW_Dummy + 2)   /* (LONG) first visible line */
#define LISTVIEW_Visible        (LISTVIEW_Dummy + 3)   /* (LONG) visible lines (get) */
#define LISTVIEW_Total          (LISTVIEW_Dummy + 4)   /* (LONG) lines in the list (get) */
#define LISTVIEW_ShowSelected   (LISTVIEW_Dummy + 6)   /* (BOOL) highlight the selection */
#define LISTVIEW_Selected       (LISTVIEW_Dummy + 7)   /* (LONG) selected line */
#define LISTVIEW_SelectedNode   LISTVIEW_Selected
#define LISTVIEW_MakeVisible    (LISTVIEW_Dummy + 8)   /* (LONG) scroll this line into view */
#define LISTVIEW_MultiSelect    (LISTVIEW_Dummy + 9)   /* (BOOL) */
#define LISTVIEW_ItemHeight     (LISTVIEW_Dummy + 10)  /* (LONG) height of a line */
#define LISTVIEW_CallHook       (LISTVIEW_Dummy + 11)  /* (struct Hook *) renders the lines */
#define LISTVIEW_ScrollUp       (LISTVIEW_Dummy + 12)
#define LISTVIEW_ScrollDown     (LISTVIEW_Dummy + 13)
#define LISTVIEW_ScrollLeft     (LISTVIEW_Dummy + 14)
#define LISTVIEW_ScrollRight    (LISTVIEW_Dummy + 15)
#define LISTVIEW_Spacing        (LISTVIEW_Dummy + 16)  /* (LONG) extra space between lines */

/* Justification of a line */
#define LVJ_LEFT    0
#define LVJ_CENTER  1
#define LVJ_RIGHT   2

/* One line */
struct ListLabelNode
{
    struct Node   lvn_Node;             /* ln_Name is the text */
    APTR          lvn_UserData;
    UWORD         lvn_RenderForeground; /* pens, normal ... */
    UWORD         lvn_RenderBackground;
    UWORD         lvn_SelectForeground; /* ... and selected */
    UWORD         lvn_SelectBackground;
    struct Image *lvn_RenderImage;      /* optional images */
    struct Image *lvn_SelectImage;
    UWORD         lvn_TextLength;       /* set by the gadget */
    WORD          lvn_LeftEdge;
    WORD          lvn_TopEdge;
    WORD          lvn_Width;
    WORD          lvn_Height;
    UWORD         lvn_Justification;    /* LVJ_* */
    BOOL          lvn_Selected;
};

#endif /* GADGETS_LISTVIEW_H */
