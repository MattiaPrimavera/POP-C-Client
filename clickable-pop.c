#include "peroraison.h"

void createXTable(void *data, pop* response)
{
  int i, j;
  XTable *own = (XTable*) data;
  own->nb_tr = response->nombreMessages;
  own->nb_td = 3;

  for(i=0; i<response->nombreMessages; i++){
    createTableLine(data, response);
    for(j=0; j<3; j++){
      createTableCell(data, response);
    }
  }
}

//per creare sotto finestre
// declarer les fenetres
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
void createTableLine(void *data, pop* response)
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
void createTableCell(void *data, pop* response)
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
    /*for (j = 0; atts[j]; j+=2) { 
      if (!strcasecmp(atts[j], "style")) {
	     style_bg_color(atts[j+1], r);
      } else if (!strcasecmp(atts[j], "onmouseover")) {
	     r->onmouseover = strdup(atts[j+1]);
      } else if (!strcasecmp(atts[j], "onmouseout")) {
	     r->onmouseout = strdup(atts[j+1]);
      } else if (!strcasecmp(atts[j], "id")) {
	     r->id = strdup(atts[j+1]);
      } else if (!strcasecmp(atts[j], "colspan")) {
	     r->colspan = (!atts[j+1] ? 1 : strtol(atts[j+1], NULL, 10));
      }
    }*/
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