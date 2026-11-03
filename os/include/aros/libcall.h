#ifndef AROS_LIBCALL_H
#define AROS_LIBCALL_H
/*
    amiga-sdk: replaces AROS's aros/libcall.h for programs, which call the
    OS through the stubs in libamiga.a (see proto/ and clib/).
*/
#include <aros/system.h>
#define AROS_LIBFUNC_INIT
#define AROS_LIBFUNC_EXIT
#endif /* AROS_LIBCALL_H */
