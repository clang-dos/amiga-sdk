#ifndef LIBRARIES_MATHLIBRARY_H
#define LIBRARIES_MATHLIBRARY_H
/*
    Base of the IEEE math libraries (mathieeedoubbas.library and friends).
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef EXEC_LIBRARIES_H
#include <exec/libraries.h>
#endif

struct MathIEEEBase
{
    struct Library MathIEEEBase_LibNode;
    UBYTE          MathIEEEBase_reserved[18];
    LONG         (*MathIEEEBase_TaskOpenLib)(void);   /* called for each opener */
    LONG         (*MathIEEEBase_TaskCloseLib)(void);  /* ... and closer */
    /* the library may have more fields after these */
};

#endif /* LIBRARIES_MATHLIBRARY_H */
