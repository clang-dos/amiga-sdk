/*
 * reaction.lib: GetAttrsA, GetAttrs and GetCode.
 * Part of the Amiga SDK; see os/README.md.
 */
#include <exec/types.h>
#include <utility/tagitem.h>
#include <intuition/intuition.h>
#include <intuition/icclass.h>
#include <proto/intuition.h>
#include <clib/reaction_lib_protos.h>

/* NextTagItem() without utility.library, which V34 does not have. */
static const struct TagItem *next_tag(const struct TagItem **tp)
{
    const struct TagItem *t = *tp;

    while (t) {
        switch (t->ti_Tag) {
        case TAG_DONE:
            return *tp = NULL;
        case TAG_IGNORE:
            t++;
            break;
        case TAG_MORE:
            t = (const struct TagItem *)t->ti_Data;
            break;
        case TAG_SKIP:
            t += t->ti_Data + 1;
            break;
        default:
            *tp = t + 1;
            return t;
        }
    }
    return *tp = NULL;
}

/* Each tag's data points to where its attribute's value goes. Returns how
 * many attributes the object knew. */
ULONG GetAttrsA(Object *object, CONST struct TagItem *tags)
{
    const struct TagItem *t;
    ULONG n = 0;

    while ((t = next_tag(&tags)))
        if (GetAttr(t->ti_Tag, object, (ULONG *)t->ti_Data))
            n++;
    return n;
}

/* The Code of a message. For IDCMP_IDCMPUPDATE that is the ICSPECIAL_CODE
 * tag of the attribute list in IAddress, if it has one. */
UWORD GetCode(CONST struct IntuiMessage *imsg)
{
    if (imsg->Class == IDCMP_IDCMPUPDATE) {
        const struct TagItem *tags = (const struct TagItem *)imsg->IAddress, *t;

        while ((t = next_tag(&tags)))
            if (t->ti_Tag == ICSPECIAL_CODE)
                return (UWORD)t->ti_Data;
    }
    return imsg->Code;
}

__asm__(
    "        .text\n"
    "        .globl  GetAttrs\n"
    "GetAttrs:\n"
    "        lea     (8,%sp),%a0\n"
    "        move.l  %a0,-(%sp)\n"
    "        move.l  (8,%sp),-(%sp)\n"
    "        jsr     GetAttrsA\n"
    "        lea     (8,%sp),%sp\n"
    "        rts\n");
