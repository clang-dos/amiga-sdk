// Exercises the amiga.lib helpers in libamiga.a: lists, ports, I/O
// requests, the timer, tasks, pools, hooks and BOOPSI method calls.
// Prints one line per check and "PASS" at the end if all of them passed.
#include <stdio.h>
#include <string.h>

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/execbase.h>
#include <devices/timer.h>
#include <intuition/intuition.h>
#include <intuition/classes.h>
#include <intuition/classusr.h>
#include <utility/hooks.h>
#include <proto/exec.h>
#include <proto/intuition.h>
#include <clib/alib_protos.h>

struct IntuitionBase *IntuitionBase;
static int failed;

static void check(const char *what, int ok) {
  printf("%-28s %s\n", what, ok ? "ok" : "FAILED");
  if (!ok)
    failed = 1;
}

static ULONG hookfunc(struct Hook *h, APTR obj, ULONG *msg) {
  return (ULONG)h->h_Data + (ULONG)obj + msg[0] + msg[1];
}

static struct Task *parent;
static BYTE childsig;
static void child(void) {
  Signal(parent, 1UL << childsig);
}

#define MYM_ADD 0x5A0001
static ULONG dispatch(struct IClass *cl, Object *o, Msg msg) {
  if (msg->MethodID == MYM_ADD)
    return ((ULONG *)msg)[1] + ((ULONG *)msg)[2];
  return DoSuperMethodA(cl, o, msg);
}

int main(void) {
  struct ExecBase *eb = *(struct ExecBase **)4;
  printf("Kickstart %d.%d\n", eb->LibNode.lib_Version, eb->SoftVer);

  struct List l;
  struct Node a, b;
  NewList(&l);
  AddTail(&l, &a);
  AddTail(&l, &b);
  check("NewList", RemHead(&l) == &a && RemHead(&l) == &b && !RemHead(&l));

  struct MsgPort *port = CreatePort((CONST_STRPTR) "alibtest.port", 0);
  check("CreatePort", port && FindPort((CONST_STRPTR) "alibtest.port") == port);
  DeletePort(port);
  check("DeletePort", !FindPort((CONST_STRPTR) "alibtest.port"));

  port = CreatePort(NULL, 0);
  struct timerequest *tr = (struct timerequest *)CreateExtIO(port, sizeof(*tr));
  int ok = tr && OpenDevice((CONST_STRPTR)TIMERNAME, UNIT_VBLANK, (struct IORequest *)tr, 0) == 0;
  if (ok) {
    tr->tr_node.io_Command = TR_ADDREQUEST;
    tr->tr_time.tv_secs = 0;
    tr->tr_time.tv_micro = 50000;
    BeginIO((struct IORequest *)tr);
    ok = WaitIO((struct IORequest *)tr) == 0;
    CloseDevice((struct IORequest *)tr);
  }
  check("CreateExtIO + BeginIO", ok);
  DeleteExtIO((struct IORequest *)tr);
  DeletePort(port);
  check("TimeDelay", TimeDelay(UNIT_MICROHZ, 0, 20000) == 0);

  parent = FindTask(NULL);
  childsig = AllocSignal(-1);
  struct Task *t = CreateTask((CONST_STRPTR) "alibtest.child", 1, (APTR)child, 4096);
  ok = t && (Wait(1UL << childsig) & (1UL << childsig));
  check("CreateTask", ok);
  FreeSignal(childsig);

  APTR pool = LibCreatePool(MEMF_ANY | MEMF_CLEAR, 1024, 256);
  ok = pool != NULL;
  for (int i = 0; ok && i < 50; i++) {
    UBYTE *p = LibAllocPooled(pool, 16 + i);
    ok = p && p[0] == 0 && p[15 + i] == 0;
    if (ok && (i & 1))
      LibFreePooled(pool, p, 16 + i);
  }
  LibDeletePool(pool);
  check("LibCreatePool/AllocPooled", ok);

  check("FastRand", FastRand(1) == 2 && FastRand(0x80000000UL) == 0x1d872b41UL);
  ok = 1;
  for (int i = 0; i < 100; i++)
    if (RangeRand(10) >= 10)
      ok = 0;
  check("RangeRand", ok);

  struct Hook h = {0};
  h.h_Entry = (APTR)HookEntry;
  h.h_SubEntry = (APTR)hookfunc;
  h.h_Data = (APTR)1000;
  ULONG msg[2] = {20, 3};
  check("HookEntry + CallHookA", CallHookA(&h, (Object *)400, msg) == 1423);
  check("CallHook", CallHook(&h, (Object *)500, 30UL, 4UL) == 1534);

  char buf[16];
  check("ACrypt", ACrypt((STRPTR)buf, (CONST_STRPTR) "secret", (CONST_STRPTR) "user") == (STRPTR)buf &&
                      strlen(buf) == 11);

  IntuitionBase = (struct IntuitionBase *)OpenLibrary((CONST_STRPTR) "intuition.library", 36);
  if (IntuitionBase) {
    struct IClass *cl = MakeClass(NULL, (ClassID) "rootclass", NULL, 0, 0);
    ok = cl != NULL;
    if (ok) {
      cl->cl_Dispatcher.h_Entry = (APTR)HookEntry;
      cl->cl_Dispatcher.h_SubEntry = (APTR)dispatch;
      Object *o = NewObjectA(cl, NULL, NULL);
      ok = o != NULL;
      check("BOOPSI NewObject", ok);
      if (o) {
        check("DoMethod", DoMethod(o, MYM_ADD, 40UL, 2UL) == 42);
        ULONG m[3] = {MYM_ADD, 5, 6};
        check("DoMethodA", DoMethodA(o, (Msg)m) == 11);
        check("CoerceMethod", CoerceMethod(cl, o, MYM_ADD, 1UL, 2UL) == 3);
        DisposeObject(o);
      }
      FreeClass(cl);
    } else
      check("MakeClass", 0);
    CloseLibrary((struct Library *)IntuitionBase);
  } else
    printf("BOOPSI checks skipped (Kickstart 1.3)\n");

  printf(failed ? "FAIL\n" : "PASS\n");
  return failed ? 10 : 0;
}
