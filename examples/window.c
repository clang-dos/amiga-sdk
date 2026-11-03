// Opens a window on the Workbench screen, draws some text in it and waits
// for its close gadget. Works from the CLI and from a Workbench icon, on
// Kickstart 1.3 and later.
#include <stdio.h>

#include <exec/types.h>
#include <graphics/gfxbase.h>
#include <intuition/intuition.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/intuition.h>

struct IntuitionBase *IntuitionBase;
struct GfxBase *GfxBase;

int main(void) {
  IntuitionBase = (struct IntuitionBase *)OpenLibrary((CONST_STRPTR) "intuition.library", 33);
  GfxBase = (struct GfxBase *)OpenLibrary((CONST_STRPTR) "graphics.library", 33);
  if (!IntuitionBase || !GfxBase) {
    fprintf(stderr, "needs Kickstart 1.2 or later\n");
    return 20;
  }

  struct NewWindow nw = {
      .LeftEdge = 40,
      .TopEdge = 30,
      .Width = 320,
      .Height = 90,
      .DetailPen = (UBYTE)-1,
      .BlockPen = (UBYTE)-1,
      .IDCMPFlags = IDCMP_CLOSEWINDOW,
      .Flags = WFLG_CLOSEGADGET | WFLG_DRAGBAR | WFLG_DEPTHGADGET |
               WFLG_ACTIVATE | WFLG_SMART_REFRESH,
      .Title = (UBYTE *)"clang on AmigaOS",
      .Type = WBENCHSCREEN,
  };
  struct Window *win = OpenWindow(&nw);
  if (!win) {
    fprintf(stderr, "could not open the window\n");
    return 20;
  }

  struct RastPort *rp = win->RPort;
  static const char *lines[] = {"Built with clang", "and picolibc."};
  for (int i = 0; i < 2; i++) {
    SetAPen(rp, 1);
    Move(rp, 20, 30 + 12 * i);
    Text(rp, (STRPTR)lines[i], i == 0 ? 16 : 13);
  }

  WaitPort(win->UserPort);
  struct Message *msg;
  while ((msg = GetMsg(win->UserPort)))
    ReplyMsg(msg);
  CloseWindow(win);
  CloseLibrary((struct Library *)GfxBase);
  CloseLibrary((struct Library *)IntuitionBase);
  return 0;
}
