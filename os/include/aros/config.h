#ifndef AROS_CONFIG_H
#define AROS_CONFIG_H
/* amiga-sdk: the values for 68k AmigaOS (binary compatible). */
#define AROS_FLAVOUR_NATIVE             1
#define AROS_FLAVOUR_STANDALONE         2
#define AROS_FLAVOUR_EMULATION          4
#define AROS_FLAVOUR_LINKLIB            8
#define AROS_FLAVOUR_BINCOMPAT          16
#define AROS_DOS_PACKETS                1
#define AROS_AMIGAOS_COMPLIANCE         31
#define AROS_FLAVOUR                    (AROS_FLAVOUR_STANDALONE|AROS_FLAVOUR_BINCOMPAT)
#endif
