/*
 * amiga.lib: TimeDelay() and DoTimer().
 * Part of the Amiga SDK; see os/README.md.
 */
#include <exec/types.h>
#include <exec/io.h>
#include <devices/timer.h>
#include <proto/exec.h>
#include <clib/alib_protos.h>

LONG DoTimer(TimeVal_Type *tv, LONG unit, LONG command)
{
    struct MsgPort *port;
    struct timerequest *tr;
    LONG err = -1;

    port = CreatePort(NULL, 0);
    if (!port)
        return -1;
    tr = (struct timerequest *)CreateExtIO(port, sizeof(*tr));
    if (tr) {
        if (OpenDevice((CONST_STRPTR)TIMERNAME, unit, (struct IORequest *)tr, 0) == 0) {
            tr->tr_node.io_Command = command;
            tr->tr_time = *(struct timeval *)tv;
            err = DoIO((struct IORequest *)tr);
            *(struct timeval *)tv = tr->tr_time;
            CloseDevice((struct IORequest *)tr);
        }
        DeleteExtIO((struct IORequest *)tr);
    }
    DeletePort(port);
    return err;
}

LONG TimeDelay(LONG unit, ULONG secs, ULONG microsecs)
{
    struct timeval tv;

    tv.tv_secs = secs;
    tv.tv_micro = microsecs;
    return DoTimer((TimeVal_Type *)&tv, unit, TR_ADDREQUEST);
}
