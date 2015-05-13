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
  own.premier = NULL;
  own.dernier = NULL;
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

void initOwn(){

}

//first window appearing to the screen
void createLoginWindow(Window fen)
{
  createXLoginTable(own, &admin);
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

// declarer les sous-fenetres
void create_td_window(Window fen)
{
	liste_de_ligne *q;
  liste_de_case *p;
  int i,j;
  int width = (int) (own.width / own.nb_td);
  int height = (int) (own.height / own.nb_tr);
  //unsigned long mask;

  int counter = 0;
	for (q = own.premier, i=MARGIN; q ; q = q->next, i+=height){
    for (p = q->premier, j=MARGIN; p ; p = p->next){

	   	p->fenetre = XCreateSimpleWindow(dpy, fen,j,i,
				       (width * p->colspan) - BORDER,
				       (height * p->rowspan) - BORDER,
				       BORDER, 
				       WhitePixel(dpy,DefaultScreen(dpy)),
				       p->bgcolor.pixel);
	   	
      XSelectInput(dpy, p->fenetre, ButtonPressMask | ExposureMask);
	  	j+= (width*p->colspan);
      filles[counter] = p->fenetre;
      counter++;
      XMapWindow(dpy, p->fenetre);
  	}
  }//end for
}//end function

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
      
      XSelectInput(dpy, filles[counterX*own.nb_td + counterY], ButtonPressMask | ExposureMask);
      j+= (width);
      XMapWindow(dpy, filles[counterX*own.nb_td + counterY]);
    }
  }//end for
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
