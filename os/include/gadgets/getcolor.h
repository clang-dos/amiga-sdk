#ifndef GADGETS_GETCOLOR_H
#define GADGETS_GETCOLOR_H
/*
    getcolor.gadget: a button that opens a colour requester.
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef REACTION_REACTION_H
#include <reaction/reaction.h>
#endif
#ifndef INTUITION_GADGETCLASS_H
#include <intuition/gadgetclass.h>
#endif

#define GETCOLOR_Dummy          (REACTION_Dummy + 0x43000)
#define GETCOLOR_TitleText      (GETCOLOR_Dummy + 1)   /* (STRPTR) requester title */
#define GETCOLOR_Screen         (GETCOLOR_Dummy + 2)   /* (struct Screen *) */
#define GETCOLOR_Color          (GETCOLOR_Dummy + 3)   /* (ULONG) 0x00RRGGBB */
#define GETCOLOR_Red            (GETCOLOR_Dummy + 4)   /* (ULONG) 32-bit components */
#define GETCOLOR_Green          (GETCOLOR_Dummy + 5)
#define GETCOLOR_Blue           (GETCOLOR_Dummy + 6)
#define GETCOLOR_Hue            (GETCOLOR_Dummy + 7)
#define GETCOLOR_Saturation     (GETCOLOR_Dummy + 8)
#define GETCOLOR_Brightness     (GETCOLOR_Dummy + 9)
#define GETCOLOR_RGB            (GETCOLOR_Dummy + 10)  /* (ULONG *) red, green, blue */
#define GETCOLOR_HSB            (GETCOLOR_Dummy + 11)  /* (ULONG *) hue, saturation, brightness */
#define GETCOLOR_ColorWheel     (GETCOLOR_Dummy + 12)  /* (BOOL) show a colour wheel */
#define GETCOLOR_RGBSliders     (GETCOLOR_Dummy + 13)  /* (BOOL) show RGB sliders */
#define GETCOLOR_HSBSliders     (GETCOLOR_Dummy + 14)  /* (BOOL) show HSB sliders */
#define GETCOLOR_SwitchMode     (GETCOLOR_Dummy + 15)  /* (BOOL) let the user switch */
#define GETCOLOR_Initial        (GETCOLOR_Dummy + 16)  /* (ULONG) starting colour */
#define GETCOLOR_ShowRGB        (GETCOLOR_Dummy + 17)  /* (BOOL) show RGB values */
#define GETCOLOR_ShowHSB        (GETCOLOR_Dummy + 18)  /* (BOOL) show HSB values */
#define GETCOLOR_SmallTextAttr  (GETCOLOR_Dummy + 19)  /* (struct TextAttr *) for the values */

/* Open the requester */
#define GCOLOR_REQUEST          (0x630001L)

struct gcRequest
{
    ULONG          MethodID;
    struct Window *gcr_Window;
};

#define GetColorObject  NewObject(GETCOLOR_GetClass(), NULL
#define GetColorEnd     TAG_END)

#endif /* GADGETS_GETCOLOR_H */
