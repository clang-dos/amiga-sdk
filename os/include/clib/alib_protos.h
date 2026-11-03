#ifndef CLIB_ALIB_PROTOS_H
#define CLIB_ALIB_PROTOS_H

/*
 * amiga.lib: the link-time helper functions of AmigaOS (in libamiga.a).
 *
 * Part of the Amiga SDK headers. The prototypes are those of the AmigaOS
 * interface (as in NDK 3.2). See os/README.md.
 */

#ifndef EXEC_TYPES_H
#include <exec/types.h>
#endif
#ifndef EXEC_IO_H
#include <exec/io.h>
#endif
#ifndef DEVICES_TIMER_H
#include <devices/timer.h>
#endif
#ifndef DEVICES_KEYMAP_H
#include <devices/keymap.h>
#endif
#ifndef LIBRARIES_COMMODITIES_H
#include <libraries/commodities.h>
#endif
#ifndef UTILITY_HOOKS_H
#include <utility/hooks.h>
#endif
#ifndef INTUITION_CLASSES_H
#include <intuition/classes.h>
#endif
#ifndef INTUITION_CLASSUSR_H
#include <intuition/classusr.h>
#endif
#ifndef GRAPHICS_GRAPHINT_H
struct Isrvstr;
#endif
struct RexxMsg;

#ifdef __cplusplus
extern "C" {
#endif

/* exec */
VOID BeginIO(struct IORequest *ioReq);
struct IORequest *CreateExtIO(CONST struct MsgPort *port, LONG ioSize);
struct MsgPort *CreatePort(CONST_STRPTR name, LONG pri);
struct IOStdReq *CreateStdIO(CONST struct MsgPort *port);
struct Task *CreateTask(CONST_STRPTR name, LONG pri, APTR initPC, ULONG stackSize);
VOID DeleteExtIO(struct IORequest *ioReq);
VOID DeletePort(struct MsgPort *ioReq);
VOID DeleteStdIO(struct IOStdReq *ioReq);
VOID DeleteTask(struct Task *task);
VOID NewList(struct List *list);
APTR LibAllocPooled(APTR poolHeader, ULONG memSize);
APTR LibCreatePool(ULONG memFlags, ULONG puddleSize, ULONG threshSize);
VOID LibDeletePool(APTR poolHeader);
VOID LibFreePooled(APTR poolHeader, APTR memory, ULONG memSize);

/* math */
ULONG FastRand(ULONG seed);
UWORD RangeRand(ULONG maxValue);

/* graphics: vertical blank */
VOID AddTOF(struct Isrvstr *i, LONG (*p)(APTR args), APTR a);
VOID RemTOF(struct Isrvstr *i);
VOID waitbeam(LONG b);

/* Motorola FFP number conversions (not in this libamiga.a: the FLOAT
 * values are FFP numbers, which this compiler does not use) */
FLOAT afp(CONST_STRPTR string);
VOID arnd(LONG place, LONG exp, STRPTR string);
FLOAT dbf(ULONG exp, ULONG mant);
LONG fpa(FLOAT fnum, STRPTR string);
VOID fpbcd(FLOAT fnum, STRPTR string);

/* timer */
LONG TimeDelay(LONG unit, ULONG secs, ULONG microsecs);
LONG DoTimer(TimeVal_Type *tv, LONG unit, LONG command);

/* commodities and tool types */
VOID ArgArrayDone(VOID);
STRPTR *ArgArrayInit(LONG argc, CONST_STRPTR *argv);
LONG ArgInt(CONST_STRPTR *tt, CONST_STRPTR entry, LONG defaultval);
STRPTR ArgString(CONST_STRPTR *tt, CONST_STRPTR entry, CONST_STRPTR defaultstring);
CxObj *HotKey(CONST_STRPTR description, struct MsgPort *port, LONG id);
struct InputEvent *InvertString(CONST_STRPTR str, CONST struct KeyMap *km);
VOID FreeIEvents(struct InputEvent *events);

/* ARexx (not in this libamiga.a yet) */
BOOL CheckRexxMsg(CONST struct RexxMsg *rexxmsg);
LONG GetRexxVar(CONST struct RexxMsg *rexxmsg, CONST_STRPTR name, STRPTR *result);
LONG SetRexxVar(struct RexxMsg *rexxmsg, CONST_STRPTR name, CONST_STRPTR value, LONG length);

/* hooks and BOOPSI */
ULONG CallHookA(struct Hook *hookPtr, Object *obj, APTR message);
ULONG CallHook(struct Hook *hookPtr, Object *obj, ...);
ULONG DoMethodA(Object *obj, Msg message);
ULONG DoMethod(Object *obj, ULONG methodID, ...);
ULONG DoSuperMethodA(struct IClass *cl, Object *obj, Msg message);
ULONG DoSuperMethod(struct IClass *cl, Object *obj, ULONG methodID, ...);
ULONG CoerceMethodA(struct IClass *cl, Object *obj, Msg message);
ULONG CoerceMethod(struct IClass *cl, Object *obj, ULONG methodID, ...);
ULONG HookEntry(struct Hook *hookPtr, Object *obj, APTR message);
ULONG SetSuperAttrs(struct IClass *cl, Object *obj, ULONG tag1, ...);

/* password hashing (V39 amiga.lib) */
STRPTR ACrypt(STRPTR buffer, CONST_STRPTR password, CONST_STRPTR username);

#ifdef __cplusplus
}
#endif

#endif /* CLIB_ALIB_PROTOS_H */
