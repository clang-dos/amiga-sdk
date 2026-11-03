#ifndef LIBRARIES_KEYMAP_H
#define LIBRARIES_KEYMAP_H
/*
    keymap.library: key map structures and raw key codes.
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef DEVICES_KEYMAP_H
#include <devices/keymap.h>
#endif

/* Raw key codes of the keys that are the same on every keyboard */
#define RAWKEY_SPACE            0x40
#define RAWKEY_BACKSPACE        0x41
#define RAWKEY_TAB              0x42
#define RAWKEY_ENTER            0x43    /* numeric keypad */
#define RAWKEY_RETURN           0x44
#define RAWKEY_ESC              0x45
#define RAWKEY_DEL              0x46
#define RAWKEY_INSERT           0x47
#define RAWKEY_PAGEUP           0x48
#define RAWKEY_PAGEDOWN         0x49
#define RAWKEY_F11              0x4B
#define RAWKEY_CRSRUP           0x4C
#define RAWKEY_CRSRDOWN         0x4D
#define RAWKEY_CRSRRIGHT        0x4E
#define RAWKEY_CRSRLEFT         0x4F
#define RAWKEY_F1               0x50
#define RAWKEY_F2               0x51
#define RAWKEY_F3               0x52
#define RAWKEY_F4               0x53
#define RAWKEY_F5               0x54
#define RAWKEY_F6               0x55
#define RAWKEY_F7               0x56
#define RAWKEY_F8               0x57
#define RAWKEY_F9               0x58
#define RAWKEY_F10              0x59
#define RAWKEY_HELP             0x5F
#define RAWKEY_LSHIFT           0x60
#define RAWKEY_RSHIFT           0x61
#define RAWKEY_CAPSLOCK         0x62
#define RAWKEY_LCTRL            0x63
#define RAWKEY_LALT             0x64
#define RAWKEY_RALT             0x65
#define RAWKEY_LCOMMAND         0x66    /* left Amiga */
#define RAWKEY_RCOMMAND         0x67    /* right Amiga */
#define RAWKEY_MENU             0x6B
#define RAWKEY_PRINTSCR         0x6D
#define RAWKEY_BREAK            0x6E
#define RAWKEY_F12              0x6F
#define RAWKEY_HOME             0x70
#define RAWKEY_END              0x71
#define RAWKEY_MEDIA_STOP       0x72
#define RAWKEY_MEDIA_PLAY_PAUSE 0x73
#define RAWKEY_MEDIA_PREV_TRACK 0x74
#define RAWKEY_MEDIA_NEXT_TRACK 0x75
#define RAWKEY_MEDIA_SHUFFLE    0x76
#define RAWKEY_MEDIA_REPEAT     0x77
#define RAWKEY_WHEEL_UP         0x7A
#define RAWKEY_WHEEL_DOWN       0x7B
#define RAWKEY_WHEEL_LEFT       0x7C
#define RAWKEY_WHEEL_RIGHT      0x7D

#endif /* LIBRARIES_KEYMAP_H */
