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

void fKeyPress (XKeyEvent *e)
{
  printf("fenetre %lu en mode %d arrete\n", e->window, e->send_event);

  XFreeGC(dpy, gc);
//  XFreeGC(dpy, gcinv);
  XCloseDisplay(dpy);
  printf("%lu en mode %d arrete\n", e->window, e->send_event);
  exit(0);
}


void creerGC ()
{
  XGCValues   xgcv;
  char* f = "-*-new century schoolbook-*-*-*-*-16-*-*-*-*-*-*-*";
  if (!(font = XLoadQueryFont(dpy, f)))
    {fprintf(stderr, "police %s inconnue.\n", f);exit(1);}

  espacement = (font->ascent + font->descent)<<1;
  largeur    = espacement<<3;

  xgcv.font = font->fid;

  xgcv.foreground = BlackPixel(dpy, DefaultScreen(dpy));
  xgcv.background = WhitePixel(dpy, DefaultScreen(dpy));
  gc = XCreateGC(dpy, racine, GCForeground|GCBackground|GCFont, &xgcv);

//  xgcv.foreground = WhitePixel(dpy, DefaultScreen(dpy));
//  xgcv.background = BlackPixel(dpy, DefaultScreen(dpy));
//  gcinv = XCreateGC(dpy, racine, GCForeground|GCBackground|GCFont, &xgcv);
}