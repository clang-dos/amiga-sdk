#ifndef GADGETS_SKETCHBOARD_H
#define GADGETS_SKETCHBOARD_H
/*
    sketchboard.gadget: a small paint area (used by the icon editor).
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef REACTION_REACTION_H
#include <reaction/reaction.h>
#endif
#ifndef INTUITION_CGHOOKS_H
#include <intuition/cghooks.h>
#endif
#ifndef INTUITION_GADGETCLASS_H
#include <intuition/gadgetclass.h>
#endif

#define SKETCHBOARD_Dummy       (REACTION_Dummy + 0x24600)
#define SGA_BitMap              (SKETCHBOARD_Dummy + 0)    /* (struct BitMap *) the picture */
#define SGA_MouseX              (SKETCHBOARD_Dummy + 1)    /* (LONG) pointer position in pixels (get) */
#define SGA_MouseY              (SKETCHBOARD_Dummy + 2)
#define SGA_Width               (SKETCHBOARD_Dummy + 3)    /* (LONG) picture size */
#define SGA_Height              (SKETCHBOARD_Dummy + 4)
#define SGA_APen                (SKETCHBOARD_Dummy + 5)    /* (LONG) drawing pen */
#define SGA_Tool                (SKETCHBOARD_Dummy + 6)    /* (ULONG) SGTOOL_* */
#define SGA_UndoAvailable       (SKETCHBOARD_Dummy + 7)    /* (BOOL) (get) */
#define SGA_RedoAvailable       (SKETCHBOARD_Dummy + 8)    /* (BOOL) (get) */
#define SGA_ShowGrid            (SKETCHBOARD_Dummy + 9)    /* (BOOL) */
#define SGA_Scale               (SKETCHBOARD_Dummy + 10)   /* (LONG) zoom factor */
#define SGA_AfPt                (SKETCHBOARD_Dummy + 11)   /* (UWORD *) area fill pattern */
#define SGA_AfPtSize            (SKETCHBOARD_Dummy + 12)   /* (LONG) its size */
#define SGA_HPropTotal          (SKETCHBOARD_Dummy + 13)   /* scroller values (get) */
#define SGA_HPropTop            (SKETCHBOARD_Dummy + 14)
#define SGA_HPropVisible        (SKETCHBOARD_Dummy + 15)
#define SGA_VPropTotal          (SKETCHBOARD_Dummy + 16)
#define SGA_VPropTop            (SKETCHBOARD_Dummy + 17)
#define SGA_VPropVisible        (SKETCHBOARD_Dummy + 18)
#define SGA_HotSpot             (SKETCHBOARD_Dummy + 19)   /* (ULONG) x << 16 | y */
#define SGA_Changed             (SKETCHBOARD_Dummy + 20)   /* (BOOL) picture was changed */
#define SGA_RastPort            (SKETCHBOARD_Dummy + 21)   /* (struct RastPort *) (get) */
#define SGA_SketchBoardRect     (SKETCHBOARD_Dummy + 22)   /* (struct Rectangle *) (get) */
#define SGA_SubWidth            (SKETCHBOARD_Dummy + 23)   /* (LONG) part of the bitmap to edit */
#define SGA_SubHeight           (SKETCHBOARD_Dummy + 24)
#define SGA_DoubleScaleX        (SKETCHBOARD_Dummy + 25)   /* (BOOL) for non-square pixels */
#define SGA_DoubleScaleY        (SKETCHBOARD_Dummy + 26)
#define SGA_TopLayerBitmap      (SKETCHBOARD_Dummy + 27)   /* (struct BitMap *) overlay */
#define SGA_TopLayerWidth       (SKETCHBOARD_Dummy + 28)   /* (LONG) */
#define SGA_WithBevel           (SKETCHBOARD_Dummy + 29)   /* (BOOL) draw a frame */
#define SGA_ScrollWheelResponse (SKETCHBOARD_Dummy + 30)   /* (ULONG) SGSCROLLWHEEL_* */
#define SGA_MaxUndoSteps        (SKETCHBOARD_Dummy + 31)   /* (LONG) */
#define SGA_UpdatedBitMap       (SKETCHBOARD_Dummy + 32)   /* (struct BitMap *) */
#define SGA_MaxScale            (SKETCHBOARD_Dummy + 33)   /* (LONG) */

/* SGA_Tool */
#define SGTOOL_FREEHAND_DOTS    0
#define SGTOOL_FREEHAND         1
#define SGTOOL_ELLIPSE          2
#define SGTOOL_ELLIPSE_FILLED   3
#define SGTOOL_RECT             4
#define SGTOOL_RECT_FILLED      5
#define SGTOOL_LINE             6
#define SGTOOL_FILL             7
#define SGTOOL_GETPEN           8
#define SGTOOL_HOTSPOT          9
#define SGTOOL_SELECT           10
#define SGTOOL_MOVE             11

/* SGA_ScrollWheelResponse */
#define SGSCROLLWHEEL_NOTHING       0
#define SGSCROLLWHEEL_SCROLLANDZOOM 1
#define SGSCROLLWHEEL_ZOOM          2

/* Methods */
#define SGM_Clear               (0x580100L)
#define SGM_Undo                (0x580101L)
#define SGM_Redo                (0x580102L)
#define SGM_Scroll              (0x580103L)

/* SGM_Clear, SGM_Undo, SGM_Redo */
struct spGeneral
{
    ULONG              MethodID;
    struct GadgetInfo  GInfo;
};

/* SGM_Scroll */
struct spScrollRaster
{
    ULONG              MethodID;
    struct GadgetInfo *GInfo;
    LONG               DeltaX;
    LONG               DeltaY;
};

#define SketchBoardObject   NewObject(SKETCHBOARD_GetClass(), NULL
#define SketchBoardEnd      TAG_END)

#endif /* GADGETS_SKETCHBOARD_H */
