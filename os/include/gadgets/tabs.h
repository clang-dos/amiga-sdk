#ifndef GADGETS_TABS_H
#define GADGETS_TABS_H
/*
    tabs.gadget: a row of file-folder style tabs.
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef INTUITION_INTUITION_H
#include <intuition/intuition.h>
#endif
#ifndef INTUITION_GADGETCLASS_H
#include <intuition/gadgetclass.h>
#endif

#define TABS_Dummy      (TAG_USER + 0x04000000)
#define TABS_Labels     (TABS_Dummy + 1)    /* (TabLabel *) array, ends with a NULL label */
#define TABS_Current    (TABS_Dummy + 2)    /* (LONG) selected tab */

/* Pens of one tab */
#define TL_TEXTPEN          0
#define TL_BACKGROUNDPEN    1
#define TL_FILLTEXTPEN      2
#define TL_FILLPEN          3
#define MAX_TL_PENS         4

typedef struct tagTabLabel
{
    STRPTR  tl_Label;                  /* NULL ends the array */
    WORD    tl_Pens[MAX_TL_PENS];      /* -1: default pen */
    APTR    tl_Attrs;                  /* reserved */
} TabLabel;

#endif /* GADGETS_TABS_H */
