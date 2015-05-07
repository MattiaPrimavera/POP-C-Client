#include "peroraison.h"

int PopList(char* requete, int desc, pop* response)
{
	printf("in PopList function\n");
	if(verifieSyntaxe(requete, "LIST") != 0) return -1;

	envoieServeur(requete, desc);


	//lecture 1ere ligne
	FILE* fdesc = fdopen(desc, "r");
	char reponse[LINELENGTH];
	printf("%s\n", reponse);
	if(reponsePositive(fdesc, reponse) != 0) return -1;

	if(sscanf(reponse, "+OK %d%s\n", pop->nombreMessages, reponse))
		printf("NUMERO MEX: %d\n", pop->nombreMessages);
	
	return 0;
	//lecture Reponse
	FILE* fdesc = fdopen(desc, "r");
	char reponse[LINELENGTH];
    while (fgets(reponse, LINELENGTH, fdesc)){
    	if(sscanf(buf, "Last-Modified: %s\n", date)){  

    }

	
	printf("%s\n", reponse);

	if(reponsePositive(fdesc, reponse) != 0) return -1;
		return 0;
}  