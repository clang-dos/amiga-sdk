#ifndef REXX_STORAGE_H
#define REXX_STORAGE_H

/*
 * ARexx data structures.
 *
 * Part of the Amiga SDK headers. Names, values and layouts follow the
 * AmigaOS ARexx interface (as in NDK 3.2) so that existing programs build
 * unchanged. See os/README.md.
 */

#ifndef EXEC_PORTS_H
#include <exec/ports.h>
#endif
#ifndef EXEC_LIBRARIES_H
#include <exec/libraries.h>
#endif

/* An ARexx internal string; allocated for one length, never resized. */
struct NexxStr
{
    LONG  ns_Ivalue;            /* integer value */
    UWORD ns_Length;            /* length, without the 0 byte */
    UBYTE ns_Flags;             /* NSF_* */
    UBYTE ns_Hash;
    BYTE  ns_Buff[8];           /* the string (variable length) */
};

#define NXADDLEN    9           /* header bytes before ns_Buff, plus the 0 byte */
#define IVALUE(ns)  ((ns)->ns_Ivalue)

#define NSB_KEEP    0
#define NSB_STRING  1
#define NSB_NOTNUM  2
#define NSB_NUMBER  3
#define NSB_BINARY  4
#define NSB_FLOAT   5
#define NSB_EXT     6
#define NSB_SOURCE  7

#define NSF_KEEP    (1 << NSB_KEEP)
#define NSF_STRING  (1 << NSB_STRING)
#define NSF_NOTNUM  (1 << NSB_NOTNUM)
#define NSF_NUMBER  (1 << NSB_NUMBER)
#define NSF_BINARY  (1 << NSB_BINARY)
#define NSF_FLOAT   (1 << NSB_FLOAT)
#define NSF_EXT     (1 << NSB_EXT)
#define NSF_SOURCE  (1 << NSB_SOURCE)

#define NSF_INTNUM  (NSF_NUMBER | NSF_BINARY | NSF_STRING)
#define NSF_DPNUM   (NSF_NUMBER | NSF_FLOAT)
#define NSF_ALPHA   (NSF_NOTNUM | NSF_STRING)
#define NSF_OWNED   (NSF_SOURCE | NSF_EXT | NSF_KEEP)
#define KEEPSTR     (NSF_STRING | NSF_SOURCE | NSF_NOTNUM)
#define KEEPNUM     (NSF_STRING | NSF_SOURCE | NSF_NUMBER | NSF_BINARY)

/* An argstring: like NexxStr, but in system memory. Programs pass a
   pointer to ra_Buff. */
struct RexxArg
{
    LONG  ra_Size;              /* bytes allocated */
    UWORD ra_Length;            /* string length */
    UBYTE ra_Flags;
    UBYTE ra_Hash;
    BYTE  ra_Buff[8];           /* the string (variable length) */
};

/* The message for all communication with ARexx (128 bytes). */
struct RexxMsg
{
    struct Message  rm_Node;
    APTR            rm_TaskBlock;       /* private */
    APTR            rm_LibBase;         /* private */
    LONG            rm_Action;          /* RXCOMM, RXFUNC, ... and RXF* flags */
    LONG            rm_Result1;         /* return code */
    LONG            rm_Result2;         /* secondary result (argstring) */
    STRPTR          rm_Args[16];        /* ARG0 .. ARG15 */
    struct MsgPort *rm_PassPort;
    STRPTR          rm_CommAddr;        /* host port name */
    STRPTR          rm_FileExt;         /* macro file extension */
    LONG            rm_Stdin;           /* input filehandle */
    LONG            rm_Stdout;          /* output filehandle */
    LONG            rm_avail;
};

#define ARG0(rmp)   ((rmp)->rm_Args[0])
#define ARG1(rmp)   ((rmp)->rm_Args[1])
#define ARG2(rmp)   ((rmp)->rm_Args[2])

#define MAXRMARG    15

/* rm_Action command codes */
#define RXCOMM      0x01000000
#define RXFUNC      0x02000000
#define RXCLOSE     0x03000000
#define RXQUERY     0x04000000
#define RXADDFH     0x07000000
#define RXADDLIB    0x08000000
#define RXREMLIB    0x09000000
#define RXADDCON    0x0A000000
#define RXREMCON    0x0B000000
#define RXTCOPN     0x0C000000
#define RXTCCLS     0x0D000000

/* rm_Action modifier bits */
#define RXFB_NOIO   16
#define RXFB_RESULT 17
#define RXFB_STRING 18
#define RXFB_TOKEN  19
#define RXFB_NONRET 20
#define RXFB_SCRIPT 21

#define RXFF_NOIO   (1L << RXFB_NOIO)
#define RXFF_RESULT (1L << RXFB_RESULT)
#define RXFF_STRING (1L << RXFB_STRING)
#define RXFF_TOKEN  (1L << RXFB_TOKEN)
#define RXFF_NONRET (1L << RXFB_NONRET)
#define RXFF_SCRIPT (1L << RXFB_SCRIPT)

#define RXCODEMASK  0xFF000000
#define RXARGMASK   0x0000000F

/* A global resource node. If rr_Func is set, it is a library offset,
   called with rr_Base in a6 and the node in a0 when the node is freed. */
struct RexxRsrc
{
    struct Node rr_Node;
    WORD        rr_Func;
    APTR        rr_Base;
    LONG        rr_Size;        /* size of the node */
    LONG        rr_Arg1;
    LONG        rr_Arg2;
};

/* rr_Node.ln_Type */
#define RRT_ANY     0
#define RRT_LIB     1
#define RRT_PORT    2
#define RRT_FILE    3
#define RRT_HOST    4
#define RRT_CLIP    5

#define GLOBALSZ    200

/* ARexx's per-task data (private) */
struct RexxTask
{
    BYTE           rt_Global[GLOBALSZ];
    struct MsgPort rt_MsgPort;
    UBYTE          rt_Flags;    /* RTF* */
    BYTE           rt_SigBit;
    APTR           rt_ClientID;
    APTR           rt_MsgPkt;
    APTR           rt_TaskID;
    APTR           rt_RexxPort;
    APTR           rt_ErrTrap;
    APTR           rt_StackPtr;
    struct List    rt_Header1;  /* environments */
    struct List    rt_Header2;  /* memory free list */
    struct List    rt_Header3;  /* memory allocations */
    struct List    rt_Header4;  /* files */
    struct List    rt_Header5;  /* message ports */
};

#define RTFB_TRACE  0
#define RTFB_HALT   1
#define RTFB_SUSP   2
#define RTFB_TCUSE  3
#define RTFB_WAIT   6
#define RTFB_CLOSE  7

#define MEMQUANT    16L
#define MEMMASK     0xFFFFFFF0
#define MEMQUICK    (1L << 0)
#define MEMCLEAR    (1L << 16)

struct SrcNode
{
    struct SrcNode *sn_Succ;
    struct SrcNode *sn_Pred;
    APTR            sn_Ptr;
    LONG            sn_Size;
};

/* Non-zero if msg is a message from ARexx (needs RexxSysBase and the
   IsRexxMsg() prototype). */
#define CHECKREXXMSG(msg) (RexxSysBase && (msg)->rm_LibBase == RexxSysBase && \
                           (msg)->rm_TaskBlock && IsRexxMsg(msg))

#endif /* REXX_STORAGE_H */
