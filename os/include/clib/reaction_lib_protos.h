#ifndef CLIB_REACTION_LIB_PROTOS_H
#define CLIB_REACTION_LIB_PROTOS_H

/*
 * reaction.lib: link-time helpers for ReAction programs (in libamiga.a).
 * Each helper calls the class library it works with, so the program must
 * open that library first (ListBrowserBase, ClickTabBase, ChooserBase,
 * RadioButtonBase, LayoutBase).
 *
 * Part of the Amiga SDK headers. See os/README.md.
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef EXEC_LISTS_H
#include <exec/lists.h>
#endif
#ifndef INTUITION_INTUITION_H
#include <intuition/intuition.h>
#endif
#ifndef INTUITION_CLASSUSR_H
#include <intuition/classusr.h>
#endif
#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* listbrowser.gadget: change the list of a gadget that is on show */
struct Node *LBAddNodeA(struct Gadget *lb, struct Window *w, struct Requester *r, struct Node *n, CONST struct TagItem *tags);
struct Node *LBAddNode(struct Gadget *lb, struct Window *w, struct Requester *r, struct Node *n, ...);
ULONG LBEditNodeA(struct Gadget *lb, struct Window *w, struct Requester *r, struct Node *n, CONST struct TagItem *tags);
ULONG LBEditNode(struct Gadget *lb, struct Window *w, struct Requester *r, struct Node *n, ...);
ULONG LBRemNode(struct Gadget *lb, struct Window *w, struct Requester *r, struct Node *n);

/* Node lists from NULL-terminated label arrays; free them with Free*() */
struct List *BrowserNodesA(STRPTR *labels);
struct List *BrowserNodes(STRPTR label1, ...);
VOID FreeBrowserNodes(struct List *list);
struct List *ChooserLabelsA(STRPTR *labels);
struct List *ChooserLabels(STRPTR label1, ...);
VOID FreeChooserLabels(struct List *list);
struct List *ClickTabsA(STRPTR *labels);
struct List *ClickTabs(STRPTR label1, ...);
VOID FreeClickTabs(struct List *list);
struct List *RadioButtonsA(STRPTR *labels);
struct List *RadioButtons(STRPTR label1, ...);
VOID FreeRadioButtons(struct List *list);

/* BOOPSI and IDCMP */
ULONG GetAttrsA(Object *object, CONST struct TagItem *tags);
ULONG GetAttrs(Object *object, Tag tag1, ...);
UWORD GetCode(CONST struct IntuiMessage *imsg);
ULONG LibDoGadgetMethodA(struct Gadget *gad, struct Window *win, struct Requester *req, Msg msg);
ULONG LibDoGadgetMethod(struct Gadget *gad, struct Window *win, struct Requester *req, ULONG methodID, ...);

/* layout.gadget: open a window sized to fit a layout */
struct Window *OpenLayoutWindowTagList(struct Gadget *layout, struct Screen *screen, CONST struct TagItem *tags);
struct Window *OpenLayoutWindowTags(struct Gadget *layout, struct Screen *screen, ...);

#ifdef __cplusplus
}
#endif

#endif /* CLIB_REACTION_LIB_PROTOS_H */
