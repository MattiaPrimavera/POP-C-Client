#include "pop.h"

int PopUser(char* requete, int desc, pop* response)
{
  if (verifieSyntaxe(requete, "USER") != 0) return -1;
  envoieServeur(requete, desc);

  //lecture Reponse
  FILE* fdesc = fdopen(desc, "r");
  char reponse[LINELENGTH];
  fflush(NULL);
  if (reponsePositive(fdesc, &reponse[0]) != 0) {
    return -1;
  }
  //printf("REPONSE:%s\n", reponse);
  return 0;
}

int PopPass(char* requete, int desc, pop* response)
{
  if (verifieSyntaxe(requete, "PASS") != 0) return -1;
  envoieServeur(requete, desc);

  //lecture Reponse
  FILE* fdesc = fdopen(desc, "r");
  char reponse[LINELENGTH];
  printf("%s", reponse);

  if (reponsePositive(fdesc, reponse) != 0) return -1;
  return 0;
}

int PopQuit(char* requete, int desc, pop* response)
{
  if (verifieSyntaxe(requete, "QUIT") != 0) return -1;
  envoieServeur(requete, desc);

  //lecture Reponse
  FILE* fdesc = fdopen(desc, "r");
  char reponse[LINELENGTH];

  if (reponsePositive(fdesc, reponse) != 0) return -1;
  printf("%s", reponse);
  //correcte sortie du programme
  printf("Vous avez choisi de Quitter l'application...\n");
  printf("Merci pour avoir utiliser notre Client POP.\n");
  exit(EXIT_SUCCESS);
}

int PopMuet(char* requete, int desc, pop* response)
{
  return -1;
}

int PopTop(char* requete, int desc, pop* response)
{
  if (verifieSyntaxe(requete, "TOP") != 0) return -1;
  envoieServeur(requete, desc);

  //lecture 1ere ligne
  FILE* fdesc = fdopen(desc, "r");
  char reponse[LINELENGTH];
  printf("%s", reponse);
  if (reponsePositive(fdesc, reponse) != 0) return -1;

  //recuperation id message
  int mexId = 0;
  if (!sscanf(requete, "TOP %d 0\n", &mexId)) {
    printf("Erreur recuperation ID\n");
    return -1;
  }
  //printf("MEX ID inserted: %d\n", mexId);
  //if(mexId <= 0) return -1;

  AnalyseEntetes(requete, mexId, fdesc, response);
  //sortie avec erreur
  return -1;
}

int PopRetr(char* requete, int desc, pop* response)
{
  if (verifieSyntaxe(requete, "RETR") != 0) return -1;
  envoieServeur(requete, desc);

  //lecture 1ere ligne
  FILE* fdesc = fdopen(desc, "r");
  char reponse[LINELENGTH];
  printf("%s", reponse);
  if (reponsePositive(fdesc, &reponse[0]) != 0) return -1;

  //recuperation id message
  int mexId = 0;
  if (!sscanf(requete, "RETR %d\n", &mexId)) {
    printf("Erreur recuperation ID\n");
    return -1;
  }
  //if(mexId <= 0) return -1;

  AnalyseEntetes(requete, mexId, fdesc, response);
  return 0;
}

int PopList(char* requete, int desc, pop* response)
{
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
      return 0;
    }

    //creation et remplissage du message, ajout à la liste
    message* mex = (message*)malloc(sizeof(message));
    sscanf(reponse, "%d %d\n", &mex->id, &mex->taille);
    //printf("mex id : %d\nmex taille : %d\n", mex->id, mex->taille);
    addMessage(response, mex);
  }

  //sortie avec erreur
  return -1;
}
