#include "pop.h"

void createRetrWindow(int mexId)
{
    //message* mex = findById(mexId, &response);
    retrWinList[mexId].mexId = mexId;

    //CREATING RETR WINDOW
    XColor bgcolor;
    bgcolor.pixel = 25000;
    screen = DefaultScreen(dpy);
    Window retrWin = XCreateSimpleWindow(dpy, XRootWindow(dpy, screen), 0, 0,
                         500,
                         500,
                         BORDER,
                         WhitePixel(dpy, DefaultScreen(dpy)),
                         bgcolor.pixel);

    //CREATING SUB-RIGHT-WINDOW
    XColor subColor;
    subColor.pixel = 50000;
    Window retrWinSub = XCreateSimpleWindow(dpy, retrWin, 450, 0,
                        50,
                        500,
                        BORDER,
                        WhitePixel(dpy, DefaultScreen(dpy)),
                        subColor.pixel);

    //SENSIBILIZATION AND MAPPING
    XSelectInput(dpy, retrWin, ExposureMask);
    XSelectInput(dpy, retrWinSub, ExposureMask | ButtonPressMask);

    XMapWindow(dpy, retrWin);
    XMapWindow(dpy, retrWinSub);
    XFlush(dpy);

    retrWinList[mexId].main = retrWin;
    retrWinList[mexId].scroll = retrWinSub;
}
