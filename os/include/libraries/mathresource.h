#ifndef LIBRARIES_MATHRESOURCE_H
#define LIBRARIES_MATHRESOURCE_H
/*
    MathIEEE.resource: lets a math coprocessor board supply its own
    versions of the IEEE math library functions.
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef EXEC_NODES_H
#include <exec/nodes.h>
#endif

struct MathIEEEResource
{
    struct Node MathIEEEResource_Node;
    UWORD       MathIEEEResource_Flags;      /* MATHIEEERESOURCEF_* */
    UWORD      *MathIEEEResource_BaseAddr;   /* the board's registers */
    void      (*MathIEEEResource_DblBasInit)(void);
    void      (*MathIEEEResource_DblTransInit)(void);
    void      (*MathIEEEResource_SglBasInit)(void);
    void      (*MathIEEEResource_SglTransInit)(void);
    void      (*MathIEEEResource_ExtBasInit)(void);
    void      (*MathIEEEResource_ExtTransInit)(void);
};

/* Which init routines the resource provides */
#define MATHIEEERESOURCEB_DBLBAS    0
#define MATHIEEERESOURCEB_DBLTRANS  1
#define MATHIEEERESOURCEB_SGLBAS    2
#define MATHIEEERESOURCEB_SGLTRANS  3
#define MATHIEEERESOURCEB_EXTBAS    4
#define MATHIEEERESOURCEB_EXTTRANS  5

#define MATHIEEERESOURCEF_DBLBAS    (1<<MATHIEEERESOURCEB_DBLBAS)
#define MATHIEEERESOURCEF_DBLTRANS  (1<<MATHIEEERESOURCEB_DBLTRANS)
#define MATHIEEERESOURCEF_SGLBAS    (1<<MATHIEEERESOURCEB_SGLBAS)
#define MATHIEEERESOURCEF_SGLTRANS  (1<<MATHIEEERESOURCEB_SGLTRANS)
#define MATHIEEERESOURCEF_EXTBAS    (1<<MATHIEEERESOURCEB_EXTBAS)
#define MATHIEEERESOURCEF_EXTTRANS  (1<<MATHIEEERESOURCEB_EXTTRANS)

#endif /* LIBRARIES_MATHRESOURCE_H */
