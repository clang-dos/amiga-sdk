#ifndef DOS_SHELL_H
#define DOS_SHELL_H
/*
    Shell extensions of AmigaOS 3.2: command history and line input.
    Part of the clang-dos Amiga SDK; written for it.
*/

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef EXEC_NODES_H
#include <exec/nodes.h>
#endif
#ifndef UTILITY_TAGITEM_H
#include <utility/tagitem.h>
#endif
#ifndef UTILITY_HOOKS_H
#include <utility/hooks.h>
#endif
#ifndef DOS_DOS_H
#include <dos/dos.h>
#endif
#ifndef DOS_DOSEXTENS_H
#include <dos/dosextens.h>
#endif

/* Methods of DoShellMethod() */
#define SHELL_METH_METHODS  ((ULONG)'L'<<24 | (ULONG)'I'<<16 | (ULONG)'S'<<8 | (ULONG)'T')   /* list the methods */
#define SHELL_METH_FGETS    ((ULONG)'G'<<24 | (ULONG)'E'<<16 | (ULONG)'T'<<8 | (ULONG)'S')   /* read a line with editing */
#define SHELL_METH_ADDHIST  ((ULONG)'A'<<24 | (ULONG)'D'<<16 | (ULONG)'D'<<8 | (ULONG)'H')   /* add a history line */
#define SHELL_METH_GETHIST  ((ULONG)'G'<<24 | (ULONG)'E'<<16 | (ULONG)'T'<<8 | (ULONG)'H')   /* get the history */
#define SHELL_METH_CLRHIST  ((ULONG)'C'<<24 | (ULONG)'L'<<16 | (ULONG)'R'<<8 | (ULONG)'H')   /* clear the history */

/* Tags */
#define SHELL_DUMMY         (TAG_USER + 3000)
#define SHELL_FGETS_FULL    (SHELL_DUMMY + 1)
#define SHELL_ADDH_LINE     (SHELL_DUMMY + 2)

/* One line of the command history */
struct HistoryNode
{
    struct MinNode hn_Node;
    UBYTE          hn_Line[1];      /* the line, NUL-terminated */
};

/* The CLI structure of a 3.2 shell: struct CommandLineInterface plus more */
struct ExtendedCommandLineInterface
{
    LONG        cle_Result2;        /* IoErr() of the last command */
    BSTR        cle_SetName;        /* current directory name */
    BPTR        cle_CommandDir;     /* command search path */
    LONG        cle_ReturnCode;     /* return code of the last command */
    BSTR        cle_CommandName;    /* name of the running command */
    LONG        cle_FailLevel;      /* FailAt */
    BSTR        cle_Prompt;
    BPTR        cle_StandardInput;
    BPTR        cle_CurrentInput;
    BSTR        cle_CommandFile;    /* script being run */
    LONG        cle_Interactive;
    LONG        cle_Background;
    BPTR        cle_CurrentOutput;
    LONG        cle_DefaultStack;   /* in longwords */
    BPTR        cle_StandardOutput;
    BPTR        cle_Module;         /* seglist of the running command */
    /* 3.2 additions */
    struct Hook cle_Hook;           /* called for the shell methods */
    APTR        cle_This;           /* private */
    ULONG       cle_Version;        /* version of this structure */
};

#endif /* DOS_SHELL_H */
