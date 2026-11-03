#ifndef GRAPHICS_DISPLAY_H
#define GRAPHICS_DISPLAY_H
/*
    Old (V1.x) custom chip display register bits.
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif

/* bplcon0 */
#define MODE_640    0x8000
#define PLNCNTMSK   0x7         /* number of bitplanes, 3 bits ... */
#define PLNCNTSHFT  12          /* ... at bit 12 */
#define PF2PRI      0x40        /* bplcon2: playfield 2 in front */
#define COLORON     0x0200      /* colour burst on */
#define DBLPF       0x400       /* dual playfield */
#define HOLDNMODIFY 0x800       /* HAM */
#define INTERLACE   4

/* bplcon1 */
#define PFA_FINE_SCROLL         0xF
#define PFB_FINE_SCROLL_SHIFT   4
#define PF_FINE_SCROLL_MASK     0xF

/* display window */
#define DIW_HORIZ_POS           0x7F
#define DIW_VRTCL_POS           0x1FF
#define DIW_VRTCL_POS_SHIFT     7

/* data fetch */
#define DFTCH_MASK              0xFF

/* vposr */
#define VPOSRLOF                0x8000  /* long frame */

#endif /* GRAPHICS_DISPLAY_H */
