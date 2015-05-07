#include "peroraison.h"

char argv0[128] = "";

void peroraison(char *f, char *m, int n)
{
	printf("%s: %s :", argv0, f);
	perror(m);
	exit(n);
}

/*
void printSvnStruct(svn *own){
	if(!own) {printf("structure own null\n"); return;}

	printf("own->server:%s\n", own->server);
	printf("own->serverpath:%s\n", own->serverpath);
	printf("own->revision:%s\n", own->revision);
	printf("own->committed_rev:%s\n", own->committed_rev);
	printf("own->committed_date:%s\n", own->committed_date);
	printf("own->last_author:%s\n", own->last_author);

	//if ((entries*)(own->contenu) == NULL)  reponse = "NIENTE CONTENUTO";
	//else reponse = "CONTENUTO PRESENTE";
}*/

void printAtts(const char** atts)
{
  int j;
  for (j = 0; atts[j]; j++) printf("atts[%d]->%s\n", j, atts[j]);
}