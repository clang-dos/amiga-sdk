/*
 * amiga.lib: exec helpers (ports, I/O requests, tasks, lists, pools).
 * Part of the Amiga SDK (README.md, "amiga.lib functions").
 */
#include <exec/types.h>
#include <exec/memory.h>
#include <exec/ports.h>
#include <exec/io.h>
#include <exec/tasks.h>
#include <exec/execbase.h>
#include <proto/exec.h>
#include <clib/alib_protos.h>

void NewList(struct List *list)
{
    list->lh_Head = (struct Node *)&list->lh_Tail;
    list->lh_Tail = NULL;
    list->lh_TailPred = (struct Node *)&list->lh_Head;
}

struct MsgPort *CreatePort(CONST_STRPTR name, LONG pri)
{
    struct MsgPort *port;
    BYTE sig = AllocSignal(-1);

    if (sig == -1)
        return NULL;
    port = AllocMem(sizeof(*port), MEMF_PUBLIC | MEMF_CLEAR);
    if (!port) {
        FreeSignal(sig);
        return NULL;
    }
    port->mp_Node.ln_Name = (char *)name;
    port->mp_Node.ln_Pri = pri;
    port->mp_Node.ln_Type = NT_MSGPORT;
    port->mp_Flags = PA_SIGNAL;
    port->mp_SigBit = sig;
    port->mp_SigTask = FindTask(NULL);
    if (name)
        AddPort(port);          /* also initialises the message list */
    else
        NewList(&port->mp_MsgList);
    return port;
}

void DeletePort(struct MsgPort *port)
{
    if (!port)
        return;
    if (port->mp_Node.ln_Name)
        RemPort(port);
    port->mp_Node.ln_Type = 0xff;
    port->mp_MsgList.lh_Head = (struct Node *)-1;
    FreeSignal(port->mp_SigBit);
    FreeMem(port, sizeof(*port));
}

struct IORequest *CreateExtIO(CONST struct MsgPort *port, LONG size)
{
    struct IORequest *io;

    if (!port)
        return NULL;
    io = AllocMem(size, MEMF_PUBLIC | MEMF_CLEAR);
    if (io) {
        io->io_Message.mn_Node.ln_Type = NT_REPLYMSG;
        io->io_Message.mn_ReplyPort = (struct MsgPort *)port;
        io->io_Message.mn_Length = size;
    }
    return io;
}

void DeleteExtIO(struct IORequest *io)
{
    if (!io)
        return;
    io->io_Message.mn_Node.ln_Type = 0xff;
    io->io_Device = (struct Device *)-1;
    io->io_Unit = (struct Unit *)-1;
    FreeMem(io, io->io_Message.mn_Length);
}

struct IOStdReq *CreateStdIO(CONST struct MsgPort *port)
{
    return (struct IOStdReq *)CreateExtIO(port, sizeof(struct IOStdReq));
}

void DeleteStdIO(struct IOStdReq *io)
{
    DeleteExtIO((struct IORequest *)io);
}

/* The task and its stack are in one MemList on tc_MemEntry, so that
   RemTask() frees them. */
struct Task *CreateTask(CONST_STRPTR name, LONG pri, APTR initPC, ULONG stackSize)
{
    struct {
        struct MemList ml;
        struct MemEntry stack;  /* ml_ME[1] */
    } want;
    struct MemList *ml;
    struct Task *task;
    UBYTE *stack;

    stackSize = (stackSize + 3) & ~3UL;
    want.ml.ml_NumEntries = 2;
    want.ml.ml_ME[0].me_Reqs = MEMF_PUBLIC | MEMF_CLEAR;
    want.ml.ml_ME[0].me_Length = sizeof(struct Task);
    want.stack.me_Reqs = MEMF_CLEAR;
    want.stack.me_Length = stackSize;
    ml = AllocEntry(&want.ml);
    if ((ULONG)ml & 0x80000000UL)
        return NULL;
    task = ml->ml_ME[0].me_Addr;
    stack = ml->ml_ME[1].me_Addr;
    task->tc_SPLower = stack;
    task->tc_SPUpper = stack + stackSize;
    task->tc_SPReg = stack + stackSize;
    task->tc_Node.ln_Type = NT_TASK;
    task->tc_Node.ln_Pri = pri;
    task->tc_Node.ln_Name = (char *)name;
    NewList(&task->tc_MemEntry);
    AddHead(&task->tc_MemEntry, &ml->ml_Node);
    AddTask(task, initPC, NULL);
    return task;
}

void DeleteTask(struct Task *task)
{
    RemTask(task);
}

/* --- memory pools ----------------------------------------------------------
   Exec has pools from V39; before that, each allocation is a separate
   AllocMem() on the pool's list. */

struct alib_pool {
    struct MinList list;
    ULONG flags;
};

struct alib_chunk {
    struct MinNode node;
    ULONG size;
    ULONG pad;                  /* keeps the memory 8-byte aligned */
};

static int exec_pools(void)
{
    return SysBase->LibNode.lib_Version >= 39;
}

APTR LibCreatePool(ULONG flags, ULONG puddleSize, ULONG threshSize)
{
    struct alib_pool *pool;

    if (exec_pools())
        return CreatePool(flags, puddleSize, threshSize);
    if (threshSize > puddleSize)
        return NULL;
    pool = AllocMem(sizeof(*pool), MEMF_PUBLIC);
    if (pool) {
        NewList((struct List *)&pool->list);
        pool->flags = flags;
    }
    return pool;
}

void LibDeletePool(APTR p)
{
    struct alib_pool *pool = p;
    struct alib_chunk *c;

    if (exec_pools()) {
        DeletePool(p);
        return;
    }
    if (!pool)
        return;
    while ((c = (struct alib_chunk *)RemHead((struct List *)&pool->list)))
        FreeMem(c, c->size);
    FreeMem(pool, sizeof(*pool));
}

APTR LibAllocPooled(APTR p, ULONG size)
{
    struct alib_pool *pool = p;
    struct alib_chunk *c;

    if (exec_pools())
        return AllocPooled(p, size);
    if (!pool || !size)
        return NULL;
    size += sizeof(*c);
    c = AllocMem(size, pool->flags);
    if (!c)
        return NULL;
    c->size = size;
    AddTail((struct List *)&pool->list, (struct Node *)&c->node);
    return c + 1;
}

void LibFreePooled(APTR p, APTR mem, ULONG size)
{
    struct alib_chunk *c;

    if (exec_pools()) {
        FreePooled(p, mem, size);
        return;
    }
    if (!p || !mem)
        return;
    c = (struct alib_chunk *)mem - 1;
    Remove((struct Node *)&c->node);
    FreeMem(c, c->size);
}

/* --- random numbers (the generator of amiga.lib; as in AROS) ------------- */

ULONG FastRand(ULONG seed)
{
    ULONG a = seed << 1;

    if ((LONG)seed <= 0)
        a ^= 0x1d872b41;
    return a;
}

ULONG RangeSeed;

UWORD RangeRand(ULONG maxValue)
{
    ULONG a = RangeSeed;
    UWORD i = maxValue - 1;

    do {
        ULONG b = a;
        a <<= 1;
        if ((LONG)b <= 0)
            a ^= 0x1d872b41;
    } while ((i >>= 1));
    RangeSeed = a;
    if ((UWORD)maxValue)
        return (UWORD)(((ULONG)(UWORD)a * (UWORD)maxValue) >> 16);
    return (UWORD)a;
}
