#ifndef PREFS_REACTION_H
#define PREFS_REACTION_H
/*
    ReAction preferences file (ENV:Sys/reaction.prefs).
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef LIBRARIES_IFFPARSE_H
#include <libraries/iffparse.h>
#endif
#ifndef GRAPHICS_TEXT_H
#include <graphics/text.h>
#endif
#ifndef GRAPHICS_GFX_H
#include <graphics/gfx.h>
#endif

#define ID_RACT     MAKE_ID('R','A','C','T')

#define FONTNAMESIZE 128

struct ReactionPrefs
{
    UWORD           rp_BevelType;       /* look of the frames */
    UWORD           rp_GlyphType;       /* look of the glyphs */
    UWORD           rp_LayoutSpacing;   /* space between gadgets */
    BOOL            rp_3DProp;          /* 3D scroller knobs */
    UWORD           rp_LabelPen;
    UWORD           rp_LabelPlace;
    BOOL            rp_3DLabel;
    BOOL            rp_SimpleRefresh;   /* windows use simple refresh */
    BOOL            rp_3DLook;
    struct TextAttr rp_FallbackAttr;    /* font when the screen font is too big */
    struct TextAttr rp_LabelAttr;       /* font of labels */
    UBYTE           rp_FallbackName[FONTNAMESIZE];
    UBYTE           rp_LabelName[FONTNAMESIZE];
    UBYTE           rp_Pattern[256];    /* backfill pattern file */
};

#endif /* PREFS_REACTION_H */
