/*
 * reaction.lib: OpenLayoutWindowTagList and OpenLayoutWindowTags
 * (needs layout.gadget open).
 * Part of the Amiga SDK; see os/README.md.
 */
#include <exec/types.h>
#include <utility/tagitem.h>
#include <intuition/intuition.h>
#include <intuition/gadgetclass.h>
#include <gadgets/layout.h>
#include <proto/intuition.h>
#include <proto/layout.h>
#include <clib/reaction_lib_protos.h>

static BOOL has_tag(const struct TagItem *t, Tag tag)
{
    while (t) {
        if (t->ti_Tag == TAG_DONE)
            break;
        if (t->ti_Tag == TAG_MORE)
            t = (const struct TagItem *)t->ti_Data;
        else if (t->ti_Tag == TAG_SKIP)
            t += t->ti_Data + 1;
        else if (t->ti_Tag == tag)
            return TRUE;
        else
            t++;
    }
    return FALSE;
}

/* Opens a window on screen (or on the default public screen if NULL) whose
 * inside fits the layout's smallest size, unless the tags give a size, and
 * puts the layout inside the window's borders. */
struct Window *OpenLayoutWindowTagList(struct Gadget *layout, struct Screen *screen,
                                       CONST struct TagItem *tags)
{
    struct LayoutLimits lim;
    struct Screen *s = screen;
    struct Window *win;
    WORD bw, bh;

    if (!s && !(s = LockPubScreen(NULL)))
        return NULL;
    LayoutLimits(layout, &lim, s->RastPort.Font, s);
    {
        BOOL w = has_tag(tags, WA_Width) || has_tag(tags, WA_InnerWidth);
        BOOL h = has_tag(tags, WA_Height) || has_tag(tags, WA_InnerHeight);
        struct TagItem wt[] = {
            { w ? TAG_IGNORE : WA_InnerWidth, lim.MinWidth },
            { h ? TAG_IGNORE : WA_InnerHeight, lim.MinHeight },
            { screen ? WA_CustomScreen : WA_PubScreen, (ULONG)s },
            { TAG_MORE, (ULONG)tags },
        };
        win = OpenWindowTagList(NULL, wt);
    }
    if (!screen)
        UnlockPubScreen(NULL, s);
    if (!win)
        return NULL;

    bw = win->BorderLeft + win->BorderRight;
    bh = win->BorderTop + win->BorderBottom;
    {
        struct TagItem gt[] = {
            { GA_Left, win->BorderLeft },
            { GA_Top, win->BorderTop },
            { GA_RelWidth, -bw },
            { GA_RelHeight, -bh },
            { TAG_DONE, 0 },
        };
        SetAttrsA(layout, gt);
    }
    WindowLimits(win, lim.MinWidth + bw, lim.MinHeight + bh,
                 lim.MaxWidth + bw > 0xffff ? 0xffff : lim.MaxWidth + bw,
                 lim.MaxHeight + bh > 0xffff ? 0xffff : lim.MaxHeight + bh);
    AddGList(win, layout, -1, 1, NULL);
    RefreshGList(layout, win, NULL, 1);
    return win;
}

__asm__(
    "        .text\n"
    "        .globl  OpenLayoutWindowTags\n"
    "OpenLayoutWindowTags:\n"
    "        lea     (12,%sp),%a0\n"
    "        move.l  %a0,-(%sp)\n"
    "        move.l  (12,%sp),-(%sp)\n"
    "        move.l  (12,%sp),-(%sp)\n"
    "        jsr     OpenLayoutWindowTagList\n"
    "        lea     (12,%sp),%sp\n"
    "        rts\n");
