#include "peroraison.h"

int PopList(char* requete, int desc, pop* response)
{
  printf("in PopList function\n");
  if (verifieSyntaxe(requete, "LIST") != 0) return -1;

  envoieServeur(requete, desc);

  //lecture 1ere ligne
  FILE* fdesc = fdopen(desc, "r");
  char reponse[LINELENGTH];
  printf("%s", reponse);
  if (reponsePositive(fdesc, reponse) != 0) return -1;

  cleanPop(response);
  //lecture corps reponse
  while (fgets(reponse, LINELENGTH, fdesc)) {
    printf("%s", reponse);

    //condition d'arret
    if (!strncmp(".", reponse, strlen("."))) {
      printf("Numero messaggi trovati: %d\n", response->nombreMessages);
      //printf("2eme message:\nid = %d\ntaille = %d\n", response->listeMessages->next->id, response->listeMessages->next->taille);
      return 0;
    }

    //creation et remplissage du message, ajout à la liste
    message* mex = (message*)malloc(sizeof(message));
    sscanf(reponse, "%d %d\n", &mex->id, &mex->taille);
    printf("mex id : %d\nmex taille : %d\n", mex->id, mex->taille);
    addMessage(response, mex);

  }

  //sortie avec erreur
  return -1;
}

/*
while (fgets(reponse, LINELENGTH, fdesc)) {
  if (sscanf(buf, "Last-Modified: %s\n", date)) {
    if (sscanf(reponse, "+OK %d%s\n", &response->nombreMessages, reponse))
      printf("NUMERO MEX: %d\n", response->nombreMessages);

  }


  printf("%s\n", reponse);

  if (reponsePositive(fdesc, reponse) != 0) return -1;
  return 0;
}
*/
