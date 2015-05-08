#include "peroraison.h"

int PopRetr(char* requete, int desc, pop* response)
{
	printf("in PopRetr function\n");
	if(verifieSyntaxe(requete, "RETR") != 0) return -1;

	envoieServeur(requete, desc);

	//lecture 1ere ligne
	FILE* fdesc = fdopen(desc, "r");
	char reponse[LINELENGTH];
	printf("REPONSE:%s", reponse);
	if(reponsePositive(fdesc, &reponse[0]) != 0) return -1;

	//recuperation id message
	int mexId = 0;
	if(!sscanf(requete, "RETR %d\n", &mexId)){
		printf("ERRORE SCANSIONE ID\n");
		return -1;
	}
	printf("MEX ID inserted: %d\n", mexId);
	//if(mexId <= 0) return -1;

	//printf("ID messaggio1 : %d\n", response->listeMessages->id);
	//interpretation en-tetes
	AnalyseEntetes(mexId, fdesc, response);

	char* tmp = reponse;
	//sauvegarde du corps
	while(fgets(tmp, LINELENGTH, fdesc)){
		printf("tmp:%s", tmp);
	    if(!strncmp(".", tmp, strlen("."))){
      		printf("\nfin CORPS ...\n");
      		break;
    	}
    	tmp = tmp + strlen(tmp);
	}
	//printf("COOOOOOOOOORPS: \n\n%s\n", reponse);

    //sortie avec erreur
	return -1;
}