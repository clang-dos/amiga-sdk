// amiga.lib: hooks and BOOPSI method calls, and BeginIO().
//
// Part of the Amiga SDK (README.md, "amiga.lib functions").
//
// Hooks get the hook in a0, the object in a2 and the message in a1; C
// functions take their arguments on the stack and may clobber d0-d1/a0-a1.

        .text

// ULONG HookEntry(hook a0, object a2, message a1)
// Puts in h_Entry for a C function in h_SubEntry:
//   ULONG func(struct Hook *, APTR object, APTR message)
        .globl  HookEntry
HookEntry:
        move.l  %a1,-(%sp)
        move.l  %a2,-(%sp)
        move.l  %a0,-(%sp)
        move.l  (12,%a0),%a0  // h_SubEntry
        jsr     (%a0)
        lea     (12,%sp),%sp
        rts

// ULONG CallHookA(struct Hook *hook, Object *obj, APTR message)
        .globl  CallHookA
CallHookA:
        movem.l %a2/%a6,-(%sp)
        move.l  (12,%sp),%a0  // hook
        move.l  (16,%sp),%a2  // object
        move.l  (20,%sp),%a1  // message
        move.l  (8,%a0),%a6  // h_Entry (a6 is free for the hook)
        jsr     (%a6)
        movem.l (%sp)+,%a2/%a6
        rts

// ULONG CallHook(struct Hook *hook, Object *obj, ...)
// The message is the argument list on the stack.
        .globl  CallHook
CallHook:
        movem.l %a2/%a6,-(%sp)
        move.l  (12,%sp),%a0
        move.l  (16,%sp),%a2
        lea     (20,%sp),%a1
        move.l  (8,%a0),%a6
        jsr     (%a6)
        movem.l (%sp)+,%a2/%a6
        rts

// Dispatch msg (a1) to obj (a2) through class a0: jump to its h_Entry.
// The class is a struct IClass, which starts with its dispatcher hook.
dispatch:
        move.l  (8,%a0),%a6
        jsr     (%a6)
        movem.l (%sp)+,%a2/%a6
        rts

// ULONG DoMethodA(Object *obj, Msg msg)
        .globl  DoMethodA
DoMethodA:
        movem.l %a2/%a6,-(%sp)
        move.l  (12,%sp),%a2  // obj
        move.l  (16,%sp),%a1  // msg
        bra   ofobj

// ULONG DoMethod(Object *obj, ULONG methodID, ...)
        .globl  DoMethod
DoMethod:
        movem.l %a2/%a6,-(%sp)
        move.l  (12,%sp),%a2
        lea     (16,%sp),%a1
ofobj:
        moveq   #0,%d0
        move.l  %a2,%d1
        beq   none  // NULL object: return 0
        move.l  (-4,%a2),%a0  // OCLASS(obj): the class, before the object
        bra   dispatch
none:
        movem.l (%sp)+,%a2/%a6
        rts

// ULONG DoSuperMethodA(struct IClass *cl, Object *obj, Msg msg)
        .globl  DoSuperMethodA
DoSuperMethodA:
        movem.l %a2/%a6,-(%sp)
        move.l  (16,%sp),%a2
        move.l  (20,%sp),%a1
        bra   super

// ULONG DoSuperMethod(struct IClass *cl, Object *obj, ULONG methodID, ...)
        .globl  DoSuperMethod
DoSuperMethod:
        movem.l %a2/%a6,-(%sp)
        move.l  (16,%sp),%a2
        lea     (20,%sp),%a1
super:
        moveq   #0,%d0
        move.l  (12,%sp),%a0  // cl
        move.l  %a0,%d1
        beq   none
        move.l  %a2,%d1
        beq   none
        move.l  (24,%a0),%a0  // cl_Super
        bra   dispatch

// ULONG CoerceMethodA(struct IClass *cl, Object *obj, Msg msg)
        .globl  CoerceMethodA
CoerceMethodA:
        movem.l %a2/%a6,-(%sp)
        move.l  (16,%sp),%a2
        move.l  (20,%sp),%a1
        bra   coerce

// ULONG CoerceMethod(struct IClass *cl, Object *obj, ULONG methodID, ...)
        .globl  CoerceMethod
CoerceMethod:
        movem.l %a2/%a6,-(%sp)
        move.l  (16,%sp),%a2
        lea     (20,%sp),%a1
coerce:
        moveq   #0,%d0
        move.l  (12,%sp),%a0
        move.l  %a0,%d1
        beq   none
        move.l  %a2,%d1
        beq   none
        bra   dispatch

// ULONG SetSuperAttrs(struct IClass *cl, Object *obj, Tag tag1, ...)
// Sends OM_SET with the tags on the stack to the superclass.
        .globl  SetSuperAttrs
SetSuperAttrs:
        movem.l %a2/%a6,-(%sp)
        move.l  (16,%sp),%a2
        lea     (20,%sp),%a0  // the tag list
        clr.l   -(%sp)  // ops_GInfo = NULL
        move.l  %a0,-(%sp)  // ops_AttrList
        move.l  #0x103,-(%sp)  // OM_SET
        move.l  %sp,%a1
        moveq   #0,%d0
        move.l  (24,%sp),%a0  // cl
        move.l  %a0,%d1
        beq ssa_done
        move.l  %a2,%d1
        beq ssa_done
        move.l  (24,%a0),%a0  // cl_Super
        move.l  (8,%a0),%a6
        jsr     (%a6)
ssa_done:
      lea     (12,%sp),%sp
        movem.l (%sp)+,%a2/%a6
        rts

// void BeginIO(struct IORequest *io): calls the device's BEGINIO vector.
        .globl  BeginIO
BeginIO:
        move.l  %a6,-(%sp)
        move.l  (8,%sp),%a1
        move.l  (20,%a1),%a6  // io_Device
        jsr     (-30,%a6)
        move.l  (%sp)+,%a6
        rts
