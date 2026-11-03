/*
 * amiga.lib: waitbeam. In its own file, since it needs GfxBase.
 * Part of the Amiga SDK; see os/README.md.
 */
#include <exec/types.h>
#include <proto/graphics.h>
#include <clib/alib_protos.h>

/* Busy-waits until the video beam reaches line b. */
VOID waitbeam(LONG b)
{
    while (VBeamPos() < b)
        ;
}
