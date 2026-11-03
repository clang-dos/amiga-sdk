#ifndef TOOLS_TEXTEDIT_FILETYPES_H
#define TOOLS_TEXTEDIT_FILETYPES_H
/*
    TextEdit file type plugins (syntax highlighting per file type).
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef TOOLS_TEXTEDIT_PLUGIN_H
#include <tools/TextEdit/plugin.h>
#endif
#ifndef GADGETS_TEXTEDITOR_H
#include <gadgets/texteditor.h>
#endif
#ifndef UTILITY_HOOKS_H
#include <utility/hooks.h>
#endif

#define FILETYPEPLUGINHEAD_ID   0x46545950UL   /* 'FTYP' */
#define FILETYPEPLUGIN_VERSION  2

/* Editor settings of a file type */
struct EditorSettings
{
    UWORD Version;
    UWORD HasLineNumbers       : 1;
    UWORD CutCopyLineWhenNoSel : 1;
    UWORD AutoIndent           : 1;
    UWORD TabKeyPolicy         : 2;     /* GV_TEXTEDITOR_TabKey_* */
    ULONG WrapColumn;
    UBYTE WordWrapMode;
    UBYTE IndentationWidth;
    UBYTE TabWidth;
    UBYTE IndentUsing;                  /* tabs or spaces */
};

/* A file type plugin: struct Plugin followed by this */
struct Filetype
{
    struct MinNode         Node;
    struct Filetype       *WorkingCopy;     /* used by the settings window */
    ULONG                  menuNumber;
    struct MenuItem       *menuitem;
    UWORD                  Version;         /* FILETYPEPLUGIN_VERSION */
    STRPTR                 name;            /* shown in the menu */
    STRPTR                 typeName;        /* used in the settings file */
    STRPTR                 autoFileTypes;   /* file name patterns */
    struct EditorSettings *editorSettings;
    /* Set by TextEdit */
    BOOL                 (*installChangeListener)(struct ChangeListener *listener);
    LONG                 (*executeRexxCommand)(STRPTR command);
    /* Set by the plugin */
    APTR                   pluginData;
    VOID                 (*terminate)(struct Filetype *ft);
    struct Hook           *highlighterHook;
    Object                *settingsGadget;
    STRPTR                 settingsTitle;
    BOOL                 (*loadSettings)(struct Filetype *ft, APTR iff);
    BOOL                 (*saveSettings)(struct Filetype *ft, APTR iff);
    VOID                 (*setSettingsFromGUI)(struct Filetype *ft);
    VOID                 (*setGUIFromSettings)(struct Filetype *ft);
    BOOL                 (*processGadgetUp)(struct Filetype *ft, UWORD gadgetID, UWORD code);
    VOID                 (*applySettings)(struct Filetype *ft, Object *editor);
    VOID                 (*disposeGadgets)(struct Filetype *ft);
    VOID                 (*assignedToDocument)(struct Filetype *ft, Object *editor);
    VOID                 (*unassignedFromDocument)(struct Filetype *ft, Object *editor);
};

#endif /* TOOLS_TEXTEDIT_FILETYPES_H */
