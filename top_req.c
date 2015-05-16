#include "peroraison.h"

int PopTop(char* requete, int desc, pop* response)
{
  printf("in PopTop function\n");
  if(verifieSyntaxe(requete, "TOP") != 0) return -1;

  envoieServeur(requete, desc);

  //lecture 1ere ligne
  FILE* fdesc = fdopen(desc, "r");
  char reponse[LINELENGTH];
  printf("%s", reponse);
  if(reponsePositive(fdesc, reponse) != 0) return -1;

  //recuperation id message
  int mexId = 0;
  if(!sscanf(requete, "TOP %d 0\n", &mexId)){
    printf("ERRORE SCANSIONE ID\n");
    return -1;
  }
  printf("MEX ID inserted: %d\n", mexId);
  //if(mexId <= 0) return -1;

  //printf("ID messaggio1 : %d\n", response->listeMessages->id);
  //interpretation en-tetes
  AnalyseEntetes(requete, mexId, fdesc, response);

    //sortie avec erreur
  return -1;
}
