#ifndef REXX_REXXIO_H
#define REXX_REXXIO_H

/*
 * ARexx input/output structures.
 *
 * Part of the Amiga SDK headers. Names, values and layouts follow the
 * AmigaOS interface (as in NDK 3.2). See os/README.md.
 */

#ifndef REXX_STORAGE_H
#include <rexx/storage.h>
#endif

#define RXBUFFSZ    204

/* A File List node (256 bytes) */
struct IoBuff
{
    struct RexxRsrc iobNode;
    APTR            iobRpt;     /* read/write position */
    LONG            iobRct;     /* characters left */
    LONG            iobDFH;     /* DOS filehandle */
    APTR            iobLock;    /* DOS lock */
    LONG            iobBct;     /* buffer length */
    BYTE            iobArea[RXBUFFSZ];
};

/* access modes */
#define RXIO_EXIST  -1
#define RXIO_STRF   0
#define RXIO_READ   1
#define RXIO_WRITE  2
#define RXIO_APPEND 3

/* SeekF() anchors */
#define RXIO_BEGIN  -1L
#define RXIO_CURR   0L
#define RXIO_END    1L

/* Library List nodes */
#define LLOFFSET(rrp)   ((rrp)->rr_Arg1)
#define LLVERS(rrp)     ((rrp)->rr_Arg2)

/* Clip List nodes: the value is an argstring in rr_Arg1 */
#define CLVALUE(rrp)    ((STRPTR)(rrp)->rr_Arg1)

/* A message port kept as a resource node */
struct RexxMsgPort
{
    struct RexxRsrc rmp_Node;
    struct MsgPort  rmp_Port;
    struct List     rmp_ReplyList;      /* received, not yet replied */
};

/* DOS device types */
#define DT_DEV      0L
#define DT_DIR      1L
#define DT_VOL      2L

/* private packet types */
#define ACTION_STACK 2002L
#define ACTION_QUEUE 2003L

#endif /* REXX_REXXIO_H */
