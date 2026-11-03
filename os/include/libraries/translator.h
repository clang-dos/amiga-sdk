#ifndef LIBRARIES_TRANSLATOR_H
#define LIBRARIES_TRANSLATOR_H
/*
    translator.library error codes (Translate() returns these).
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

#define TR_NotUsed  (-1)    /* this phoneme is not used */
#define TR_NoMem    (-2)    /* out of memory */
#define TR_MakeBad  (-4)    /* can't build the output */

#endif /* LIBRARIES_TRANSLATOR_H */
