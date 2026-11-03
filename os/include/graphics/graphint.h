#ifndef GRAPHICS_GRAPHINT_H
#define GRAPHICS_GRAPHINT_H
/*
    Old graphics interrupt server node (graphics.library private use).
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef EXEC_NODES_H
#include <exec/nodes.h>
#endif

struct Isrvstr
{
    struct Node     is_Node;
    struct Isrvstr *Iptr;       /* passed to the server */
    LONG          (*code)(void);
    LONG          (*ccode)(APTR);   /* called with Carg */
    APTR            Carg;
};

#endif /* GRAPHICS_GRAPHINT_H */
