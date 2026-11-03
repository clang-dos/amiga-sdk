#ifndef GRAPHICS_COERCE_H
#define GRAPHICS_COERCE_H
/*
    Flags for CoerceMode() (graphics.library V39).
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#define PRESERVE_COLORS 1   /* keep the colour count */
#define AVOID_FLICKER   2   /* don't pick an interlaced mode */
#define IGNORE_MCOMPAT  4   /* ignore monitor compatibility */

#define BIDTAG_COERCE   1   /* private */

#endif /* GRAPHICS_COERCE_H */
