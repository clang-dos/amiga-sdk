#ifndef AROS_ASMCALL_H
#define AROS_ASMCALL_H
/*
    amiga-sdk: replaces AROS's aros/asmcall.h for programs.

    AROS's headers declare functions that take their arguments in
    registers with these macros. Here they declare ordinary C prototypes
    (for the function pointer types in the headers); a function that the
    OS calls with arguments in registers is written as in the NDK, for
    example through HookEntry in libamiga.a, or with SDI_hook.h.
*/
#include <aros/system.h>

#define AROS_UFPA(type, name, reg) type name
#define AROS_UFCA(type, value, reg) value
#define AROS_USERFUNC_INIT
#define AROS_USERFUNC_EXIT

#define AROS_UFP0(t, n) t n(void)
#define AROS_UFP0S(t, n) static t n(void)
#define AROS_UFP1(t, n,a1) t n(a1)
#define AROS_UFP1S(t, n,a1) static t n(a1)
#define AROS_UFP2(t, n,a1,a2) t n(a1,a2)
#define AROS_UFP2S(t, n,a1,a2) static t n(a1,a2)
#define AROS_UFP3(t, n,a1,a2,a3) t n(a1,a2,a3)
#define AROS_UFP3S(t, n,a1,a2,a3) static t n(a1,a2,a3)
#define AROS_UFP4(t, n,a1,a2,a3,a4) t n(a1,a2,a3,a4)
#define AROS_UFP4S(t, n,a1,a2,a3,a4) static t n(a1,a2,a3,a4)
#define AROS_UFP5(t, n,a1,a2,a3,a4,a5) t n(a1,a2,a3,a4,a5)
#define AROS_UFP5S(t, n,a1,a2,a3,a4,a5) static t n(a1,a2,a3,a4,a5)
#define AROS_UFP6(t, n,a1,a2,a3,a4,a5,a6) t n(a1,a2,a3,a4,a5,a6)
#define AROS_UFP6S(t, n,a1,a2,a3,a4,a5,a6) static t n(a1,a2,a3,a4,a5,a6)
#define AROS_UFP7(t, n,a1,a2,a3,a4,a5,a6,a7) t n(a1,a2,a3,a4,a5,a6,a7)
#define AROS_UFP7S(t, n,a1,a2,a3,a4,a5,a6,a7) static t n(a1,a2,a3,a4,a5,a6,a7)
#define AROS_UFP8(t, n,a1,a2,a3,a4,a5,a6,a7,a8) t n(a1,a2,a3,a4,a5,a6,a7,a8)
#define AROS_UFP8S(t, n,a1,a2,a3,a4,a5,a6,a7,a8) static t n(a1,a2,a3,a4,a5,a6,a7,a8)
#define AROS_UFP9(t, n,a1,a2,a3,a4,a5,a6,a7,a8,a9) t n(a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define AROS_UFP9S(t, n,a1,a2,a3,a4,a5,a6,a7,a8,a9) static t n(a1,a2,a3,a4,a5,a6,a7,a8,a9)
#define AROS_UFP10(t, n,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) t n(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)
#define AROS_UFP10S(t, n,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) static t n(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10)
#define AROS_UFP11(t, n,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11) t n(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)
#define AROS_UFP11S(t, n,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11) static t n(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11)
#define AROS_UFP12(t, n,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12) t n(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)
#define AROS_UFP12S(t, n,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12) static t n(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12)
#define AROS_UFP13(t, n,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13) t n(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13)
#define AROS_UFP13S(t, n,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13) static t n(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13)
#define AROS_UFP14(t, n,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14) t n(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14)
#define AROS_UFP14S(t, n,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14) static t n(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14)
#define AROS_UFP15(t, n,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15) t n(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15)
#define AROS_UFP15S(t, n,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15) static t n(a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11,a12,a13,a14,a15)

#endif /* AROS_ASMCALL_H */
