#include "peroraison.h"

void createXLoginTable(void *data, user* admin){
  int i, j;
  XTable *own = (XTable*) data;
  own->nb_tr = 2;
  own->nb_td = 1;

  for(i=0; i<own->nb_tr; i++){
    createTableLine(data);
    for(j=0; j<own->nb_td; j++){
      createTableCell(data);
    }
  }  
}

void createXTable(void *data, pop* response){
  int i, j;
  XTable *own = (XTable*) data;
  own->nb_tr = response->nombreMessages;
  own->nb_td = 3;

  for(i=0; i<own->nb_tr; i++){
    createTableLine(data);
    for(j=0; j<own->nb_td; j++){
      createTableCell(data);
    }
  }  
}

void createLoginWindow(XTable *own, Window fen)
{
  int i,j=MARGIN;
  int width = (int) (own->width / own->nb_td);
  int height = (int) (own->height / own->nb_tr);

  int count;
  /*for (count = 0, i=MARGIN; count < 2 ; count++, i+=height){
      loginWin[count] = XCreateSimpleWindow(dpy, fen,j,i,
               (width * 1) - BORDER,
               (height * 1) - BORDER,
               BORDER, 
               WhitePixel(dpy,DefaultScreen(dpy)),
               own->premier->premier->bgcolor.pixel);
      XSelectInput(dpy, loginWin[count], KeyPressMask | ExposureMask | ButtonPressMask);
      XDrawString(dpy, loginWin[count,] gc, j, i, "CIAO", strlen("CIAO"));
  }*/

  for (count = 0, i=MARGIN; count < 2 ; count++, i+=height){
    screen = DefaultScreen(dpy);
    visual = DefaultVisual(dpy,screen); 
    depth  = DefaultDepth(dpy,screen); 
    attributes.background_pixel      = XWhitePixel(dpy,screen); 
    attributes.border_pixel          = XBlackPixel(dpy,screen); 
    attributes.override_redirect     = 0; 
    
    loginWin[count] = XCreateWindow(dpy, fen,
    j,i, (width * 1) - BORDER, (height * 1) - BORDER , BORDER, depth, InputOutput, visual,
    CWBackPixel | CWBorderPixel | CWOverrideRedirect,&attributes);
    XSelectInput(dpy, loginWin[count], KeyPressMask | ExposureMask | ButtonPressMask);
    //XDrawString(dpy, loginWin[count], gc, j, i, "CIAOOOOOOOOOOOOOOOOOOO", strlen("CIAOOOOOOOOOOOOOOOOOOO"));

  }


}

// declarer les sous-fenetres
void create_td_window(XTable *own, Window fen)
{
	liste_de_ligne *q;
  liste_de_case *p;
  int i,j;
  int width = (int) (own->width / own->nb_td);
  int height = (int) (own->height / own->nb_tr);
  unsigned long mask;

	for (q = own->premier, i=MARGIN; q ; q = q->next, i+=height){
    for (p = q->premier, j=MARGIN; p ; p = p->next){

	   	p->fenetre = XCreateSimpleWindow(dpy, fen,j,i,
				       (width * p->colspan) - BORDER,
				       (height * p->rowspan) - BORDER,
				       BORDER, 
				       WhitePixel(dpy,DefaultScreen(dpy)),
				       p->bgcolor.pixel);
	   	mask = 0;
	   	if (p->onmouseover) mask |= EnterWindowMask;
	   	if (p->onmouseout)  mask |= LeaveWindowMask;
	   	if (mask)	XSelectInput(dpy,p->fenetre,mask|ButtonPressMask);
	  	j+= (width*p->colspan);
  	}
  }
}

//per creare le linee
void createTableLine(void *data)
{
  XTable *own = (XTable *) data;

  liste_de_ligne *r = (liste_de_ligne *) malloc(sizeof(liste_de_ligne));
  if (r == NULL){
    fprintf(stderr, "malloc indisponible"); 
    exit(3);
  }
  r->next = NULL;
  r->premier = NULL;
  r->dernier = NULL;

  add_line(own, r);
}

//per creare le celle 
void createTableCell(void *data)
{
  XTable *own = (XTable *) data;

  liste_de_case *r = (liste_de_case *) malloc(sizeof(liste_de_case));
  if (r == NULL){
    fprintf(stderr, "malloc indisponible"); 
    exit(2);
  }
  r->onmouseover =  NULL;
  r->onmouseout =  NULL;
  r->rowspan = 1;
  r->colspan = 1;
  r->id =  NULL;
  r->next =  NULL;
  add_cell(own, r);
  if (!own->nb_tr) own->nb_td += r->colspan;
}

void add_line(XTable *t, liste_de_ligne *r)
{
      if (t->dernier)
	   t->dernier->next = r;
      else t->premier = r;
      t->dernier = r;
}

void add_cell(XTable *t, liste_de_case *r)
{
      if (t->dernier->dernier)
	   t->dernier->dernier->next = r;
      else t->dernier->premier = r;
      t->dernier->dernier = r;
}

void createMainWindow(XTable* own, Window* racine){
  XGCValues gr_values; 
  //setbuf (stdout, NULL); 
  //setbuf (stderr, NULL); 
  screen = DefaultScreen(dpy);
  visual = DefaultVisual(dpy,screen); 
  depth  = DefaultDepth(dpy,screen); 
  attributes.background_pixel      = XWhitePixel(dpy,screen); 
  attributes.border_pixel          = XBlackPixel(dpy,screen); 
  attributes.override_redirect     = 0; 
  
  *racine = XCreateWindow(dpy, XRootWindow(dpy,screen),
   200,200, own->width+50, own->height+50,5, depth, InputOutput, visual,
   CWBackPixel | CWBorderPixel | CWOverrideRedirect,&attributes);
  
  /*racine = XCreateWindow(dpy, XRootWindow(dpy,screen), 0, 0, 
             own->width + (MARGIN<<1), 
             own->height + (MARGIN<<1),
             0,
             BlackPixel(dpy,DefaultScreen(dpy)),
             WhitePixel(dpy,DefaultScreen(dpy)));*/
  
  fontinfo = XLoadQueryFont(dpy, "-*-*-*-*-*-*-20-*-*-*-*-*-*-*");
  //fontinfo = XLoadQueryFont(dpy,"6x10"); 
  gr_values.font =   fontinfo->fid; 
  gr_values.function =   GXcopy; 
  gr_values.plane_mask = AllPlanes; 
  gr_values.foreground = BlackPixel(dpy,screen); 
  gr_values.background = WhitePixel(dpy,screen); 
  gc=XCreateGC(dpy,*racine, 
    GCFont | GCFunction | GCPlaneMask | GCForeground | GCBackground,
    &gr_values); 
  
    XSelectInput(dpy,*racine,ExposureMask | ButtonPressMask | KeyPressMask);
  //XSelectInput(dpy,racine,KeyPressMask | ExposureMask);
  //XDefineCursor(dpy, racine, XCreateFontCursor(dpy,XC_heart)); 
  XMapWindow(dpy,*racine); 
  XFlush(dpy); 



}
