/*
 * amiga.lib: commodities helpers. They use commodities.library: the
 * program opens it as CxBase.
 * Part of the Amiga SDK; see os/README.md.
 */
#include <exec/types.h>
#include <exec/memory.h>
#include <devices/inputevent.h>
#include <devices/keymap.h>
#include <libraries/commodities.h>
#include <proto/exec.h>
#include <proto/commodities.h>
#include <clib/alib_protos.h>

/* A filter for description that sends id to port and swallows the event. */
CxObj *HotKey(CONST_STRPTR description, struct MsgPort *port, LONG id)
{
    CxObj *filter, *o;

    filter = CreateCxObj(CX_FILTER, (LONG)description, 0);
    if (!filter)
        return NULL;
    o = CreateCxObj(CX_SEND, (LONG)port, id);
    if (o)
        AttachCxObj(filter, o);
    o = CreateCxObj(CX_TRANSLATE, 0, 0);
    if (o)
        AttachCxObj(filter, o);
    if (CxObjError(filter)) {
        DeleteCxObjAll(filter);
        return NULL;
    }
    return filter;
}

void FreeIEvents(struct InputEvent *ie)
{
    while (ie) {
        struct InputEvent *next = ie->ie_NextEvent;
        FreeMem(ie, sizeof(*ie));
        ie = next;
    }
}

/* Input events that type str with keymap km (NULL: the default keymap).
   "\n", "\r", "\t" and "\\" are escapes; "<description>" is one key as an
   input description (see ParseIX()). The events are in reverse order, as
   the input handler wants them. */
struct InputEvent *InvertString(CONST_STRPTR str, CONST struct KeyMap *km)
{
    struct InputEvent *list = NULL, *ie;
    char desc[128];

    while (str && *str) {
        ie = AllocMem(sizeof(*ie), MEMF_PUBLIC | MEMF_CLEAR);
        if (!ie)
            goto fail;
        ie->ie_NextEvent = list;
        list = ie;
        ie->ie_Class = IECLASS_RAWKEY;
        if (*str == '<') {
            IX ix;
            int n = 0;
            str++;
            while (*str && *str != '>' && n < (int)sizeof(desc) - 1)
                desc[n++] = *str++;
            desc[n] = 0;
            if (*str == '>')
                str++;
            ix.ix_Version = IX_VERSION;
            ix.ix_Class = ix.ix_Code = ix.ix_CodeMask = 0;
            ix.ix_Qualifier = ix.ix_QualMask = ix.ix_QualSame = 0;
            if (ParseIX((CONST_STRPTR)desc, &ix) < 0)
                goto fail;
            ie->ie_Class = ix.ix_Class;
            ie->ie_Code = ix.ix_Code;
            ie->ie_Qualifier = ix.ix_Qualifier;
        } else {
            UBYTE c = *str++;
            if (c == '\\') {
                c = *str ? *str++ : '\\';
                if (c == 'n' || c == 'r')
                    c = '\n';
                else if (c == 't')
                    c = '\t';
            }
            if (!InvertKeyMap(c, ie, (struct KeyMap *)km))
                goto fail;
        }
    }
    return list;
fail:
    FreeIEvents(list);
    return NULL;
}
