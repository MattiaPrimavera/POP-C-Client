#include "peroraison.h"

void fEnter(XCrossingEvent *e, XTable *own)
{
  /*liste_de_case *p, *r;

  p = findCaseFromWindow(e->window, own);

  if (p && (r = findCaseFromId(p->onmouseover, own)))
    {
       XUnmapWindow(dpy, r->fenetre);
    }*/
}

void fLeave(XCrossingEvent *e, XTable *own) {
  /*liste_de_case *p, *r;
  p = findCaseFromWindow(e->window, own);
  if (p && (r = findCaseFromId(p->onmouseout, own)))
    {
       XMapWindow(dpy, r->fenetre);
    }*/
}

void fKeyPress ()
{
  printf("EVENEMENT KEYSTROKE\n");
  //XFreeGC(dpy, gc);
  //XCloseDisplay(dpy);
  //exit(0);
}

void fButtonPress(XButtonEvent *e, XTable *own)
{
  loginFocus = e->window;
  if(loginFocus == loginWin[0])
    printf("Finestra USER\n");
  else if(loginFocus == loginWin[1])
    printf("Finestra PASSWD\n");
}