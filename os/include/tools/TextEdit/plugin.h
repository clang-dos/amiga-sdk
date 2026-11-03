#ifndef TOOLS_TEXTEDIT_PLUGIN_H
#define TOOLS_TEXTEDIT_PLUGIN_H
/*
    TextEdit plugins: the header at the start of every plugin file.
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef DOS_DOS_H
#include <dos/dos.h>
#endif

#define MASTER_VERSION      2
#define PLUGINHEAD_VERSION  1

/* "moveq #-1,d0 : rts", so running a plugin file does nothing */
#define PLUGINHEAD_SECURITY 0x70FF4E75

struct Plugin;

/* At the start of the plugin file's first hunk */
struct PluginHead
{
    ULONG              ph_Security;     /* PLUGINHEAD_SECURITY */
    ULONG              ph_ID;           /* kind of plugin, e.g. EXTENSIONPLUGINHEAD_ID */
    BPTR               ph_SegList;      /* filled in by TextEdit */
    struct PluginHead *ph_Next;
    UWORD              ph_Version;      /* PLUGINHEAD_VERSION */
    UWORD              ph_Reserved;
    STRPTR             ph_VersString;   /* "$VER: ..." */
    struct Plugin     *ph_FirstPlugin;  /* the plugins in this file */
};

/* One plugin; the kind of plugin decides what follows */
struct Plugin
{
    struct Plugin *p_Next;
    UWORD          p_Version;           /* of the plugin kind's structure */
    UWORD          p_MasterVersion;     /* MASTER_VERSION */
    UWORD          p_PluginVersion;     /* of this plugin */
    UWORD          p_PluginRevision;
    UWORD          p_Identifier;
    UWORD          p_Flags;
    BOOL         (*init)(struct Plugin *plugin);
};

#endif /* TOOLS_TEXTEDIT_PLUGIN_H */
