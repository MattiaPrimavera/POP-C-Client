#include "peroraison.h"

void fEnter(XCrossingEvent *e, XTable *own)
{
  liste_de_case *p, *r;

  p = findCaseFromWindow(e->window, own);

  if (p && (r = findCaseFromId(p->onmouseover, own)))
    {
       XUnmapWindow(dpy, r->fenetre);
    }
}

void fLeave(XCrossingEvent *e, XTable *own) {
  liste_de_case *p, *r;
  p = findCaseFromWindow(e->window, own);
  if (p && (r = findCaseFromId(p->onmouseout, own)))
    {
       XMapWindow(dpy, r->fenetre);
    }
}

void fKeyPress ()
{
  XCloseDisplay(dpy);
  exit(0);
}
