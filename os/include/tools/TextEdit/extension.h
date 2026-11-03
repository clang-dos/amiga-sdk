#ifndef TOOLS_TEXTEDIT_EXTENSION_H
#define TOOLS_TEXTEDIT_EXTENSION_H
/*
    TextEdit extension plugins.
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef TOOLS_TEXTEDIT_PLUGIN_H
#include <tools/TextEdit/plugin.h>
#endif
#ifndef UTILITY_HOOKS_H
#include <utility/hooks.h>
#endif
#ifndef INTUITION_CLASSUSR_H
#include <intuition/classusr.h>
#endif

#define EXTENSIONPLUGINHEAD_ID  0x45585453UL   /* 'EXTS' */
#define EXTENSIONPLUGIN_VERSION 1

/* Functions TextEdit offers to an extension */
struct TextEditInterface
{
    BOOL (*installChangeListener)(struct Hook *hook);
    BOOL (*installLeftBarHook)(struct Hook *hook);
    LONG (*executeRexxCommand)(STRPTR command);
    BOOL (*insertMenu)(APTR menu);
    BOOL (*installTopGadget)(Object *gadget);
    BOOL (*installBottomGadget)(Object *gadget);
    BOOL (*installSideGadget)(Object *gadget);
    VOID (*setWindowFormatStrings)(STRPTR title, STRPTR screenTitle);
    VOID (*activateGadget)(Object *gadget);
};

/* An extension plugin: struct Plugin followed by these fields */
struct Extension
{
    APTR                      extensionData;    /* for the extension */
    struct TextEditInterface *tei;              /* set by TextEdit */
    ULONG                     sigMask;          /* signals the extension waits for */
    VOID (*setWindowPointer)(struct Window *window);
    VOID (*onNewDocument)(Object *editor);
    VOID (*onCurrentDocumentChanged)(Object *editor);
    VOID (*onDisposeDocument)(Object *editor);
    BOOL (*handleGadgetUp)(UWORD gadgetID, UWORD code);
    BOOL (*handleRawKey)(UWORD code, UWORD qualifier);
    VOID (*handleSignal)(ULONG signals);
    VOID (*aboutToQuit)(VOID);
    VOID (*terminate)(VOID);
};

#endif /* TOOLS_TEXTEDIT_EXTENSION_H */
