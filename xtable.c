#include "peroraison.h"
/*
void createXLoginTable(){
  int i, j;
  own.nb_tr = 2;
  own.nb_td = 1;

  for(i=0; i<own.nb_tr; i++){
    createTableLine(&own);
    for(j=0; j<own.nb_td; j++){
      createTableCell(&own);
    }
  }  
}

void createXTable(){
  int i, j;
  //XTable *own = (XTable*) data;
  own.nb_tr = response.nombreMessages;
  own.nb_td = 3;

  for(i=0; i<own.nb_tr; i++){
    createTableLine(&own);
    for(j=0; j<own.nb_td; j++){
      createTableCell(&own);
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
}*/