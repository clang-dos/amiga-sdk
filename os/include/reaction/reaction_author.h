#ifndef REACTION_REACTION_AUTHOR_H
#define REACTION_REACTION_AUTHOR_H
/*
    Definitions for authors of ReAction classes.
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

/* Extra pens for 3D effects */
struct UIExtPens
{
    UWORD extp_Version;     /* of this structure */
    LONG  extp_DarkPen;     /* darker than SHADOWPEN */
    LONG  extp_LightPen;    /* lighter than SHINEPEN */
};

#endif /* REACTION_REACTION_AUTHOR_H */
