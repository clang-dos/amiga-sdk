/*
 * amiga.lib: tool type arguments (ArgArrayInit() and friends).
 * They use icon.library: the program opens it as IconBase.
 * Part of the Amiga SDK; see os/README.md.
 */
#include <exec/types.h>
#include <exec/memory.h>
#include <dos/dos.h>
#include <workbench/workbench.h>
#include <workbench/startup.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/icon.h>
#include <clib/alib_protos.h>

static struct DiskObject *argdiskobj;
static STRPTR *argcopy;
static ULONG argcopysize;

/* From the shell: a NULL-terminated copy of argv[1..]. From Workbench
   (argc 0, argv is the WBStartup): the tool types of the program's icon. */
STRPTR *ArgArrayInit(LONG argc, CONST_STRPTR *argv)
{
    if (argc) {
        LONG i;
        if (argc < 2)
            return NULL;
        argcopysize = argc * sizeof(STRPTR);
        argcopy = AllocMem(argcopysize, MEMF_PUBLIC | MEMF_CLEAR);
        if (!argcopy)
            return NULL;
        for (i = 1; i < argc; i++)
            argcopy[i - 1] = (STRPTR)argv[i];
        return argcopy;
    } else {
        struct WBStartup *wb = (struct WBStartup *)argv;
        BPTR old;
        if (!wb || wb->sm_NumArgs < 1)
            return NULL;
        old = CurrentDir(wb->sm_ArgList[0].wa_Lock);
        argdiskobj = GetDiskObject((APTR)wb->sm_ArgList[0].wa_Name);
        CurrentDir(old);
        return argdiskobj ? (STRPTR *)argdiskobj->do_ToolTypes : NULL;
    }
}

void ArgArrayDone(void)
{
    if (argcopy) {
        FreeMem(argcopy, argcopysize);
        argcopy = NULL;
    }
    if (argdiskobj) {
        FreeDiskObject(argdiskobj);
        argdiskobj = NULL;
    }
}

STRPTR ArgString(CONST_STRPTR *tt, CONST_STRPTR entry, CONST_STRPTR defaultstring)
{
    STRPTR s = tt ? FindToolType((APTR)tt, (APTR)entry) : NULL;
    return s ? s : (STRPTR)defaultstring;
}

LONG ArgInt(CONST_STRPTR *tt, CONST_STRPTR entry, LONG defaultval)
{
    STRPTR s = tt ? FindToolType((APTR)tt, (APTR)entry) : NULL;
    LONG v = defaultval;
    if (s)
        StrToLong(s, &v);
    return v;
}
