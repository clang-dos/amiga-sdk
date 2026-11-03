#ifndef CLASSES_AREXX_H
#define CLASSES_AREXX_H
/*
    arexx.class: an ARexx host port as a BOOPSI object.
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef REACTION_REACTION_H
#include <reaction/reaction.h>
#endif
#ifndef EXEC_MEMORY_H
#include <exec/memory.h>
#endif
#ifndef INTUITION_CLASSES_H
#include <intuition/classes.h>
#endif
#ifndef INTUITION_CLASSUSR_H
#include <intuition/classusr.h>
#endif
#ifndef DOS_DOS_H
#include <dos/dos.h>
#endif
#ifndef DOS_RDARGS_H
#include <dos/rdargs.h>
#endif
#ifndef REXX_STORAGE_H
#include <rexx/storage.h>
#endif
#ifndef REXX_RXSLIB_H
#include <rexx/rxslib.h>
#endif

#define AREXX_Dummy         (REACTION_Dummy + 0x30000)
#define AREXX_HostName      (AREXX_Dummy + 1)   /* (STRPTR) port name, a number is added */
#define AREXX_DefExtension  (AREXX_Dummy + 2)   /* (STRPTR) script extension, "rexx" by default */
#define AREXX_DefExtention  AREXX_DefExtension  /* old spelling */
#define AREXX_Commands      (AREXX_Dummy + 3)   /* (struct ARexxCmd *) array, ends with a NULL name */
#define AREXX_ErrorCode     (AREXX_Dummy + 4)   /* (ULONG *) RXERR_* if creation fails */
#define AREXX_SigMask       (AREXX_Dummy + 5)   /* (ULONG) signal of the port (get) */
#define AREXX_NoSlot        (AREXX_Dummy + 6)   /* (BOOL) don't add a number to the name */
#define AREXX_ReplyHook     (AREXX_Dummy + 7)   /* (struct Hook *) gets replies to AM_EXECUTE */
#define AREXX_MsgPort       (AREXX_Dummy + 8)   /* (struct MsgPort *) the port (get) */

/* Methods */
#define AM_HANDLEEVENT      (0x590001L)     /* handle pending messages */
#define AM_EXECUTE          (0x590002L)     /* run a command or script */
#define AM_FLUSH            (0x590003L)     /* drop pending replies */

/* AREXX_ErrorCode values */
#define RXERR_NO_COMMAND_LIST       (1L)
#define RXERR_NO_PORT_NAME          (2L)
#define RXERR_PORT_ALREADY_EXISTS   (3L)
#define RXERR_OUT_OF_MEMORY         (4L)

/* One command of the host */
struct ARexxCmd
{
    STRPTR  ac_Name;                /* command name; NULL ends the array */
    UWORD   ac_ID;                  /* your id */
    VOID  (*ac_Func)(struct ARexxCmd *, struct RexxMsg *);
    STRPTR  ac_ArgTemplate;         /* ReadArgs() template */
    ULONG   ac_Flags;               /* reserved */
    ULONG  *ac_ArgList;             /* parsed arguments, for ac_Func */
    LONG    ac_RC;                  /* return codes, set by ac_Func */
    LONG    ac_RC2;
    STRPTR  ac_Result;              /* result string, set by ac_Func */
};

/* AM_EXECUTE */
struct apExecute
{
    ULONG   MethodID;
    STRPTR  ape_CommandString;      /* command or script name */
    STRPTR  ape_PortName;           /* port to send to, NULL for "REXX" */
    LONG   *ape_RC;                 /* results */
    LONG   *ape_RC2;
    STRPTR *ape_Result;
    BPTR    ape_IO;                 /* input and output for scripts */
};

#ifndef AREXX_GetClass
#define ARexxObject     NewObject(AREXX_GetClass(), NULL
#endif
#define ARexxEnd        TAG_END)

#endif /* CLASSES_AREXX_H */
