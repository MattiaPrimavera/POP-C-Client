#include "peroraison.h"

char argv0[128] = "";

void peroraison(char *f, char *m, int n)
{
	printf("%s: %s :", argv0, f);
	perror(m);
	exit(n);
}

void printAtts(const char** atts)
{
  int j;
  for (j = 0; atts[j]; j++) printf("atts[%d]->%s\n", j, atts[j]);
}