#ifndef GADGETS_TEXTEDITOR_H
#define GADGETS_TEXTEDITOR_H
/*
    texteditor.gadget: a multi-line text editor (BOOPSI).
    Part of the clang-dos Amiga SDK; written for it.

    Attributes and methods share one number space; some numbers are
    used twice, once by an attribute and once by a method, or by two
    attributes from different versions.
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef UTILITY_HOOKS_H
#include <utility/hooks.h>
#endif
#ifndef INTUITION_INTUITION_H
#include <intuition/intuition.h>
#endif
#ifndef INTUITION_GADGETCLASS_H
#include <intuition/gadgetclass.h>
#endif
#ifndef DEVICES_TIMER_H
#include <devices/timer.h>
#endif
#ifndef INTUITION_IMAGECLASS_H
#include <intuition/imageclass.h>
#endif

#define TEXTEDITOR_Dummy    (0x45000)

/* Attributes */
#define GA_TEXTEDITOR_Prop_Release                 (TEXTEDITOR_Dummy + 1)
#define GA_TEXTEDITOR_Contents                     (TEXTEDITOR_Dummy + 2)
#define GA_TEXTEDITOR_PopWindow_Open               (TEXTEDITOR_Dummy + 3)
#define GA_TEXTEDITOR_CursorX                      (TEXTEDITOR_Dummy + 4)
#define GA_TEXTEDITOR_CursorY                      (TEXTEDITOR_Dummy + 5)
#define GA_TEXTEDITOR_DoubleClickHook              (TEXTEDITOR_Dummy + 6)
#define GA_TEXTEDITOR_TypeAndSpell                 (TEXTEDITOR_Dummy + 7)
#define GA_TEXTEDITOR_ExportHook                   (TEXTEDITOR_Dummy + 8)
#define GA_TEXTEDITOR_ExportWrap                   (TEXTEDITOR_Dummy + 9)
#define GA_TEXTEDITOR_FixedFont                    (TEXTEDITOR_Dummy + 10)
#define GA_TEXTEDITOR_Flow                         (TEXTEDITOR_Dummy + 11)
#define GA_TEXTEDITOR_HasChanged                   (TEXTEDITOR_Dummy + 12)
#define GA_TEXTEDITOR_Prop_DeltaFactor             (TEXTEDITOR_Dummy + 13)
#define GA_TEXTEDITOR_ImportHook                   (TEXTEDITOR_Dummy + 14)
#define GA_TEXTEDITOR_InsertMode                   (TEXTEDITOR_Dummy + 15)
#define GA_TEXTEDITOR_ImportWrap                   (TEXTEDITOR_Dummy + 16)
#define GA_TEXTEDITOR_KeyBindings                  (TEXTEDITOR_Dummy + 17)
#define GA_TEXTEDITOR_UndoAvailable                (TEXTEDITOR_Dummy + 18)
#define GA_TEXTEDITOR_RedoAvailable                (TEXTEDITOR_Dummy + 19)
#define GA_TEXTEDITOR_AreaMarked                   (TEXTEDITOR_Dummy + 20)
#define GA_TEXTEDITOR_Prop_Entries                 (TEXTEDITOR_Dummy + 21)
#define GA_TEXTEDITOR_Prop_Visible                 (TEXTEDITOR_Dummy + 22)
#define GA_TEXTEDITOR_Quiet                        (TEXTEDITOR_Dummy + 23)
#define GA_TEXTEDITOR_NumLock                      (TEXTEDITOR_Dummy + 24)
#define GA_TEXTEDITOR_ReadOnly                     (TEXTEDITOR_Dummy + 25)
#define GA_TEXTEDITOR_StyleBold                    (TEXTEDITOR_Dummy + 28)
#define GA_TEXTEDITOR_StyleItalic                  (TEXTEDITOR_Dummy + 29)
#define GA_TEXTEDITOR_StyleUnderline               (TEXTEDITOR_Dummy + 30)
#define GA_TEXTEDITOR_Prop_First                   (TEXTEDITOR_Dummy + 32)
#define GA_TEXTEDITOR_WrapBorder                   (TEXTEDITOR_Dummy + 33)
#define GA_TEXTEDITOR_Separator                    (TEXTEDITOR_Dummy + 44)
#define GA_TEXTEDITOR_HorizontalScroll             (TEXTEDITOR_Dummy + 45)
#define GA_TEXTEDITOR_Pen                          (TEXTEDITOR_Dummy + 46)
#define GA_TEXTEDITOR_ColorMap                     (TEXTEDITOR_Dummy + 47)
#define GA_TEXTEDITOR_HProp_DeltaFactor            (TEXTEDITOR_Dummy + 48)
#define GA_TEXTEDITOR_HProp_Entries                (TEXTEDITOR_Dummy + 49)
#define GA_TEXTEDITOR_HProp_First                  (TEXTEDITOR_Dummy + 50)
#define GA_TEXTEDITOR_HProp_Visible                (TEXTEDITOR_Dummy + 51)
#define GA_TEXTEDITOR_ExitHelp                     (TEXTEDITOR_Dummy + 52)
#define GA_TEXTEDITOR_TabSize                      (TEXTEDITOR_Dummy + 53)
#define GA_TEXTEDITOR_ErrorCode                    (TEXTEDITOR_Dummy + 54)
#define GA_TEXTEDITOR_IndentWidth                  (TEXTEDITOR_Dummy + 54)
#define GA_TEXTEDITOR_HighlighterHook              (TEXTEDITOR_Dummy + 55)
#define GA_TEXTEDITOR_Keymap                       (TEXTEDITOR_Dummy + 55)
#define GA_TEXTEDITOR_CursorBlinkSpeed             (TEXTEDITOR_Dummy + 56)
#define GA_TEXTEDITOR_ShowLineNumbers              (TEXTEDITOR_Dummy + 56)
#define GA_TEXTEDITOR_LeftBarHook                  (TEXTEDITOR_Dummy + 57)
#define GA_TEXTEDITOR_Length                       (TEXTEDITOR_Dummy + 58)
#define GA_TEXTEDITOR_RectangularSelections        (TEXTEDITOR_Dummy + 58)
#define GA_TEXTEDITOR_AutoIndent                   (TEXTEDITOR_Dummy + 59)
#define GA_TEXTEDITOR_CutCopyLineWhenNoSelection   (TEXTEDITOR_Dummy + 60)
#define GA_TEXTEDITOR_LineEndingImported           (TEXTEDITOR_Dummy + 61)
#define GA_TEXTEDITOR_LineEndingExport             (TEXTEDITOR_Dummy + 62)
#define GA_TEXTEDITOR_TabKeyPolicy                 (TEXTEDITOR_Dummy + 63)
#define GA_TEXTEDITOR_HorizScroller                (TEXTEDITOR_Dummy + 64)
#define GA_TEXTEDITOR_VertScroller                 (TEXTEDITOR_Dummy + 65)
#define GA_TEXTEDITOR_TextAttr                     (TEXTEDITOR_Dummy + 66)
#define GA_TEXTEDITOR_TabToSpaces                  (TEXTEDITOR_Dummy + 67)
#define GA_TEXTEDITOR_Dragging                     (TEXTEDITOR_Dummy + 68)
#define GA_TEXTEDITOR_EOLMarker                    (TEXTEDITOR_Dummy + 69)
#define GA_TEXTEDITOR_LookAhead                    (TEXTEDITOR_Dummy + 70)
#define GA_TEXTEDITOR_BevelStyle                   (TEXTEDITOR_Dummy + 71)
#define GA_TEXTEDITOR_Transparent                  (TEXTEDITOR_Dummy + 72)
#define GA_TEXTEDITOR_MaxUndoLevels                (TEXTEDITOR_Dummy + 73)
#define GA_TEXTEDITOR_MaxUndoSize                  (TEXTEDITOR_Dummy + 74)
#define GA_TEXTEDITOR_SpacesPerTAB             GA_TEXTEDITOR_TabSize

/* Methods */
#define GM_TEXTEDITOR_HandleError                  (TEXTEDITOR_Dummy + 31)
#define GM_TEXTEDITOR_AddKeyBindings               (TEXTEDITOR_Dummy + 34)
#define GM_TEXTEDITOR_ARexxCmd                     (TEXTEDITOR_Dummy + 35)
#define GM_TEXTEDITOR_ClearText                    (TEXTEDITOR_Dummy + 36)
#define GM_TEXTEDITOR_ExportText                   (TEXTEDITOR_Dummy + 37)
#define GM_TEXTEDITOR_InsertText                   (TEXTEDITOR_Dummy + 38)
#define GM_TEXTEDITOR_MacroBegin                   (TEXTEDITOR_Dummy + 39)
#define GM_TEXTEDITOR_MacroEnd                     (TEXTEDITOR_Dummy + 40)
#define GM_TEXTEDITOR_MacroExecute                 (TEXTEDITOR_Dummy + 41)
#define GM_TEXTEDITOR_Replace                      (TEXTEDITOR_Dummy + 42)
#define GM_TEXTEDITOR_Search                       (TEXTEDITOR_Dummy + 43)
#define GM_TEXTEDITOR_MarkText                     (TEXTEDITOR_Dummy + 44)
#define GM_TEXTEDITOR_BlockInfo                    (TEXTEDITOR_Dummy + 48)
#define GM_TEXTEDITOR_AddChangeListener            (TEXTEDITOR_Dummy + 49)
#define GM_TEXTEDITOR_ExportBlock                  (TEXTEDITOR_Dummy + 68)
#define GM_TEXTEDITOR_ReplaceAll                   (TEXTEDITOR_Dummy + 75)

/* GA_TEXTEDITOR_Flow */
#define GV_TEXTEDITOR_Flow_Left         0
#define GV_TEXTEDITOR_Flow_Center       1
#define GV_TEXTEDITOR_Flow_Right        2
#define GV_TEXTEDITOR_Flow_Justified    3

/* GA_TEXTEDITOR_ImportHook, GA_TEXTEDITOR_ExportHook: built-in hooks */
#define GV_TEXTEDITOR_ImportHook_Plain      0
#define GV_TEXTEDITOR_ImportHook_EMail      2
#define GV_TEXTEDITOR_ImportHook_MIME       3
#define GV_TEXTEDITOR_ImportHook_MIMEQuoted 4
#define GV_TEXTEDITOR_ExportHook_Plain      0
#define GV_TEXTEDITOR_ExportHook_EMail      1

/* Line length calculation for the built-in hooks */
#define GV_TEXTEDITOR_LengthHook_Plain  0
#define GV_TEXTEDITOR_LengthHook_ANSI   1
#define GV_TEXTEDITOR_LengthHook_HTML   2
#define GV_TEXTEDITOR_LengthHook_MAIL   3

/* GM_TEXTEDITOR_InsertText position */
#define GV_TEXTEDITOR_InsertText_Cursor 0
#define GV_TEXTEDITOR_InsertText_Top    1
#define GV_TEXTEDITOR_InsertText_Bottom 2

/* GA_TEXTEDITOR_TabKeyPolicy */
#define GV_TEXTEDITOR_TabKey_IndentsLine    0
#define GV_TEXTEDITOR_TabKey_IndentsAfter   1

/* GM_TEXTEDITOR_Search flags */
#define GF_TEXTEDITOR_Search_FromTop        (1<<0)
#define GF_TEXTEDITOR_Search_Next           (1<<1)
#define GF_TEXTEDITOR_Search_Incremental    (GF_TEXTEDITOR_Search_FromTop|GF_TEXTEDITOR_Search_Next)
#define GF_TEXTEDITOR_Search_CaseSensitive  (1<<2)
#define GF_TEXTEDITOR_Search_DOSPattern     (1<<3)
#define GF_TEXTEDITOR_Search_Backwards       (1<<4)
#define GF_TEXTEDITOR_Search_Wildstar       (1<<5)
#define GF_TEXTEDITOR_Search_WholeWord      (1<<6)
#define GF_TEXTEDITOR_Search_Word           GF_TEXTEDITOR_Search_WholeWord
#define GF_TEXTEDITOR_Search_Cyclic         (1<<7)
#define GF_TEXTEDITOR_SearchType_Mask       (GF_TEXTEDITOR_Search_FromTop|GF_TEXTEDITOR_Search_Next|GF_TEXTEDITOR_Search_Backwards)
#define GF_TEXTEDITOR_Highlight_All         GF_TEXTEDITOR_SearchType_Mask

/* GM_TEXTEDITOR_ReplaceAll flags */
#define GF_TEXTEDITOR_Replace_All           (1<<0)

/* GA_TEXTEDITOR_ErrorCode, GM_TEXTEDITOR_HandleError */
#define Error_ClipboardIsEmpty          1
#define Error_ClipboardIsNotFTXT        2
#define Error_MacroBufferIsFull         3
#define Error_MemoryAllocationFailed    4
#define Error_NoAreaMarked              5
#define Error_NoMacroDefined            6
#define Error_NothingToRedo             7
#define Error_NothingToUndo             8
#define Error_NotEnoughUndoMem          9
#define Error_StringNotFound            10
#define Error_NoBookmarkInstalled       11
#define Error_BookmarkHasBeenLost       12

/* Line endings */
#define LINEENDING_LF           0
#define LINEENDING_CR           1
#define LINEENDING_CRLF         2
#define LINEENDING_ASIMPORT     3   /* export like the imported text */

/* Separator lines (GA_TEXTEDITOR_Separator) */
#define LNSB_Top        0
#define LNSB_Middle     1
#define LNSB_Bottom     2
#define LNSB_StrikeThru 3
#define LNSB_Thick      4
#define LNSF_Top        (1<<LNSB_Top)
#define LNSF_Middle     (1<<LNSB_Middle)
#define LNSF_Bottom     (1<<LNSB_Bottom)
#define LNSF_StrikeThru (1<<LNSB_StrikeThru)
#define LNSF_Thick      (1<<LNSB_Thick)

/* Text styles used by the highlighter hook */
#define TBSTYLE_UNDERLINE   0x0001
#define TBSTYLE_BOLD        0x0002
#define TBSTYLE_ITALIC      0x0004
#define TBSTYLE_SETCOLOR    0x0008
#define TBSTYLE_SPELLERROR  0x0040
#define TBSTYLE_STYLEMASK   0x00FF
#define TBSTYLE_COLORMASK   0xFF00
#define TBSTYLE_NOTSET      0xFF00

/* GA_TEXTEDITOR_LeftBarHook commands */
#define LEFTBAR_RENDERCOMMAND   0
#define LEFTBAR_MOUSECOMMAND    1

/* GA_TEXTEDITOR_DoubleClickHook message */
struct ClickMessage
{
    STRPTR LineContents;    /* the line that was clicked */
    ULONG  ClickPosition;   /* column */
};

/* GA_TEXTEDITOR_HighlighterHook message */
struct HighlightMessage
{
    ULONG  Version;
    STRPTR Text;                /* the line to colour */
    ULONG  StatusOfPrevBlock;   /* what the hook returned for the line before */
};

/* GA_TEXTEDITOR_LeftBarHook messages */
struct LeftBarRenderMessage
{
    UWORD            Command;       /* LEFTBAR_RENDERCOMMAND */
    UWORD            Version;
    ULONG            BlockNum;      /* line number */
    ULONG            LineSubNum;    /* row within a wrapped line */
    struct RastPort *RastPort;      /* where to draw */
};

struct LeftBarMouseMessage
{
    UWORD          Command;         /* LEFTBAR_MOUSECOMMAND */
    UWORD          Version;
    ULONG          BlockNum;
    UWORD          Code;            /* from the IntuiMessage */
    UWORD          Qualifier;
    WORD           X;
    WORD           Y;
    struct TimeVal TimeStamp;
};

/* GM_TEXTEDITOR_AddChangeListener */
struct ChangeListener
{
    VOID (*onCharsInserted)(ULONG line, ULONG column, ULONG count);
    VOID (*onCharsDeleted)(ULONG line, ULONG column, ULONG count);
    VOID (*onCharEntered)(ULONG line, ULONG column, ULONG character);
    APTR Reserved1;
    APTR Reserved2;
    APTR Reserved3;
    APTR Reserved4;
    APTR Reserved5;
};

/* Method messages */
struct GP_TEXTEDITOR_ARexxCmd
{
    ULONG              MethodID;
    struct GadgetInfo *GInfo;
    STRPTR             command;
};

struct GP_TEXTEDITOR_AddChangeListener
{
    ULONG                  MethodID;
    struct ChangeListener *listener;
};

struct GP_TEXTEDITOR_BlockInfo
{
    ULONG              MethodID;
    struct GadgetInfo *GInfo;
    ULONG             *startx;
    ULONG             *starty;
    ULONG             *stopx;
    ULONG             *stopy;
};

struct GP_TEXTEDITOR_ClearText
{
    ULONG              MethodID;
    struct GadgetInfo *GInfo;
};

struct GP_TEXTEDITOR_ExportText
{
    ULONG              MethodID;
    struct GadgetInfo *GInfo;
};

struct GP_TEXTEDITOR_HandleError
{
    ULONG MethodID;
    ULONG errorcode;
};

struct GP_TEXTEDITOR_InsertText
{
    ULONG              MethodID;
    struct GadgetInfo *GInfo;
    STRPTR             text;
    LONG               pos;     /* GV_TEXTEDITOR_InsertText_* */
};

struct GP_TEXTEDITOR_MarkText
{
    ULONG              MethodID;
    struct GadgetInfo *GInfo;
    ULONG              start_crsr_x;
    ULONG              start_crsr_y;
    ULONG              stop_crsr_x;
    ULONG              stop_crsr_y;
};

struct GP_TEXTEDITOR_Replace
{
    ULONG              MethodID;
    struct GadgetInfo *GInfo;
    STRPTR             newstring;
    ULONG              flags;
};

struct GP_TEXTEDITOR_ReplaceAll
{
    ULONG              MethodID;
    struct GadgetInfo *GInfo;
    STRPTR             string;
    STRPTR             newstring;
    ULONG              flags;
};

struct GP_TEXTEDITOR_Search
{
    ULONG              MethodID;
    struct GadgetInfo *GInfo;
    STRPTR             string;
    ULONG              flags;   /* GF_TEXTEDITOR_Search_* */
};

#endif /* GADGETS_TEXTEDITOR_H */
