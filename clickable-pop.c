#include "peroraison.h"

//GLOBAL VARS
Display *dpy;
Window racine, filles[nb_LIGNES*3], loginWin[2], loginFocus;
GC gc;
XFontStruct *font;
Visual *visual; 
int screen;
int depth; 
XSetWindowAttributes attributes; 
XFontStruct *fontinfo; 
XTable own;
user admin;

void graphicalMain(int argc, char** argv){
  XEvent e;

  //INSTANTIATION DISPLAY
  if ((dpy = XOpenDisplay(NULL)) == NULL){
    fprintf(stderr, "%s: dispositif %s injoignable\n", argv[0],
      XDisplayName(NULL));
    exit(2);
  }

  //INITIALIZATION STRUCTURE OWN
  //own.premier = NULL;
  //own.dernier = NULL;
  own.nb_tr= 0;
  own.nb_td= 0;
  own.width= 500;//100;
  own.height= 300;//50;

  //STARTING GUI 
  createMainWindow();
  createLoginWindow(racine);

  XMapWindow(dpy, racine);
  XMapSubwindows(dpy, racine);
   
  while (1) {
    XNextEvent(dpy, &e);
    switch (e.type){
      case KeyPress: fKeyPress(&e.xkey, &admin); break;
      case LeaveNotify: {fLeave(&e.xcrossing, &own); break;}
      case EnterNotify: {fEnter(&e.xcrossing, &own); break;}
      case ButtonPress: {fButtonPress(&e.xbutton, &own); break;} // TME
      case Expose: {
        fExpose(&e.xexpose, &admin);
          //char* text ="POMPINONE PERCHE SONO BRAVOOOOOOOOOOOOOOOOOOOOOOO <3<3<3<3<3<3<3<3";
          //DISPLAYTEXT(loginWin[0], 50, 50, text);
        break;
      }
      default: break;
    }
  }//fin main while
  exit(0);
}

//first window appearing to the screen
void createLoginWindow(Window fen)
{
  own.nb_tr = 2;
  own.nb_td = 1;
  int i,j=MARGIN;
  int width = (int) (own.width / own.nb_td);
  int height = (int) (own.height / own.nb_tr);
  XColor bgcolor;
  bgcolor.pixel = 45000;

  int count;
  for (count = 0, i=30; count < 2 ; count++, i+=height){
      loginWin[count] = XCreateSimpleWindow(dpy, fen,j,i,
               (width * 1) - BORDER,
               (height * 1) - BORDER,
               BORDER, 
               WhitePixel(dpy,DefaultScreen(dpy)),
               bgcolor.pixel);
      XSelectInput(dpy, loginWin[count], KeyPressMask | ExposureMask | ButtonPressMask);
  }
  loginFocus = loginWin[0];
}

void createListWindow(){
  int i,j;
  
  own.nb_tr = response.nombreMessages + 1;
  own.nb_td = 3;
  
  int width = (int) (own.width / own.nb_td);
  int height = (int) (own.height / own.nb_tr);

  XColor bgcolor;
  bgcolor.pixel = 25000;

  //CREATING SUBWINDOWS GRID
  int counterX, counterY;
  for (counterX = 0, i=MARGIN; counterX < own.nb_tr ; counterX++, i+=height){
    for (counterY = 0, j=MARGIN; counterY < own.nb_td ; counterY++){
      filles[counterX*own.nb_td + counterY] = XCreateSimpleWindow(dpy, racine, j, i,
               width - BORDER,
               height - BORDER,
               BORDER, 
               WhitePixel(dpy,DefaultScreen(dpy)),
               bgcolor.pixel);
      
      XSelectInput(dpy, filles[counterX*own.nb_td + counterY], ExposureMask);
      j+= (width);
      XMapWindow(dpy, filles[counterX*own.nb_td + counterY]);
    }
  }//end for

  //CREATING QUIT BUTTON
  filles[own.nb_tr * own.nb_td] = XCreateSimpleWindow(dpy, racine, MARGIN, 
               MARGIN + height * own.nb_tr,
               (width - BORDER)* 3 + MARGIN,
               height - BORDER,
               BORDER, 
               WhitePixel(dpy,DefaultScreen(dpy)),
               bgcolor.pixel);

  XSelectInput(dpy, filles[own.nb_tr * own.nb_td], ButtonPressMask | ExposureMask);
  XMapWindow(dpy, filles[own.nb_tr * own.nb_td]);

  decoratingListWindow();
}

void decoratingListWindow(){
  int width = (int) (own.width / own.nb_td);
  int height = (int) (own.height / own.nb_tr);
  
  int x = width/2 - width/4;
  int y = height/2 - height/5;

  //INTESTATION
  DISPLAYTEXT(filles[0], x, y, "IDENTIFIANT");
  DISPLAYTEXT(filles[1], x, y, "FROM");
  DISPLAYTEXT(filles[2], x, y, "DATE");

  char idBuffer[5];
  sprintf(idBuffer, "%d", response.listeMessages->next->id);
  DISPLAYTEXT(filles[3], x, y, idBuffer);
  DISPLAYTEXT(filles[4], x, y, response.listeMessages->next->emetteur);
  DISPLAYTEXT(filles[5], x, y, response.listeMessages->next->date);

  //MESSAGE GRID
  /*char idBuffer[5];
  message* mex = response.listeMessages;
  int i, j=3;
  for(i = 0; i < response.nombreMessages; i++){
    sprintf(idBuffer, "%d", mex->id);
    DISPLAYTEXT(filles[j], x, y, idBuffer);
    DISPLAYTEXT(filles[j+1], x, y, mex->date);
    DISPLAYTEXT(filles[j+2], x, y, mex->emetteur);
    mex = mex->next;
    j++;
  }*/

  DISPLAYTEXT(filles[own.nb_tr * own.nb_td], x+1.5*width - width/4, y, "QUIT");
}

void createMainWindow(){
  XGCValues gr_values; 
  XColor bgcolor;
  bgcolor.pixel = 65000;

  //CREATING MAIN WINDOW
  screen = DefaultScreen(dpy);
  racine = XCreateSimpleWindow(dpy, XRootWindow(dpy,screen), 200, 200,
               (own.width+50),
               (own.height+50),
               BORDER, 
               WhitePixel(dpy,DefaultScreen(dpy)),
               bgcolor.pixel);
  
  //CREATING GRAPHIC CONTEXT
  fontinfo = XLoadQueryFont(dpy, "-*-*-*-*-*-*-20-*-*-*-*-*-*-*");
  gr_values.font =   fontinfo->fid; 
  gr_values.function =   GXcopy; 
  gr_values.plane_mask = AllPlanes; 
  gr_values.foreground = BlackPixel(dpy,screen); 
  gr_values.background = WhitePixel(dpy,screen); 
  gc= XCreateGC(dpy, racine, 
    GCFont | GCFunction | GCPlaneMask | GCForeground | GCBackground,
    &gr_values); 
  
  //SENSIBILIZATION AND MAPPING
  XSelectInput(dpy, racine,ExposureMask | ButtonPressMask | KeyPressMask);
  XMapWindow(dpy, racine); 
  XFlush(dpy); 
}