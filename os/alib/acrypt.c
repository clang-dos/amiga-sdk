/*
 * amiga.lib: ACrypt(), the password hash of AmigaOS (as in AROS).
 * Part of the Amiga SDK; see os/README.md.
 */
#include <exec/types.h>
#include <clib/alib_protos.h>

#define OSIZE 12

STRPTR ACrypt(STRPTR buffer, CONST_STRPTR password, CONST_STRPTR username)
{
    LONG buf[OSIZE];
    LONG i, k, d;

    if (!buffer || !password || !username)
        return NULL;
    for (i = 0; i < OSIZE; i++) {
        d = *password ? *password++ : i;
        d += *username ? *username++ : i;
        buf[i] = 'A' + d;
    }
    for (i = 0; i < OSIZE; i++) {
        for (k = 0; k < OSIZE; k++)
            buf[i] = (buf[i] + buf[OSIZE - k - 1]) % 53;
        buffer[i] = buf[i] + 'A';
    }
    buffer[OSIZE - 1] = 0;
    return buffer;
}
