#include "peroraison.h"
#include <ctype.h>

char reponse_http[BUFSIZ];

int PopMuet(char* requete, int desc, pop* response)
{
  return -1;
}

void envoieServeur(char* requete, int desc){
  if (write(desc, requete, strlen(requete)) <=0)
    peroraison("write","erreur sur la socket", 5);
  fflush(NULL);
}

int reponsePositive(FILE* fdesc, char* firstLine){
  fgets(firstLine, LINELENGTH, fdesc);
  
  if (!strncmp("+OK", firstLine, strlen("+OK")))
    return 0; //reponse +OK
  else{   
    return -1; //reponse -ERR 
  }
}

int verifieSyntaxe(char* requete, char* controle){
  if (strncmp(controle, requete, strlen(controle))){
    printf("La Requete saisie n'est pas desservi par le protocole POP.\n");
    return -1;
  }
  printf("La requete %s est bien formulee'\n", controle);
  return 0;
}

int (*actions[27])(char* requete, int desc, pop* response) = {
  &PopMuet,   &PopMuet,   &PopMuet, // @ A B
  &PopMuet,   &PopMuet,   &PopMuet, // C D E
  &PopMuet,   &PopMuet,   &PopMuet, // F G H
  &PopMuet,   &PopMuet,   &PopMuet, // I J K
  &PopMuet,   &PopMuet,   &PopMuet, // L M N
  &PopMuet,   &PopPass,   &PopQuit, // O P Q
  &PopMuet,   &PopMuet,   &PopMuet, // R S T
  &PopUser, &PopMuet,   &PopMuet, // U V W
  &PopMuet,   &PopMuet,   &PopMuet  // X Y Z
};

int main(int argc, char *argv[])
{
  int port;
  int desc;
  char in[LINELENGTH];

  //verification arguments
  if (argc < 3)
    peroraison("argc", "nombre d'arguments incorrect", 3);

  if (!sscanf(argv[2],"%d",&port)) 
    peroraison("sscanf", "argument incorrect", 4);

  //connexion au serveur
  printf("Connexion sur %s sur le port %d\n", argv[1],port);
  desc = InitConnexion(argv[1], port);

  pop response;
  while (fgets(in,LINELENGTH,stdin)){ 
    printf("REQUETE: %s\n", in);
    //fflush(NULL);

    //on divise le traitement par requetes...
    char sep = tolower(in[0]);
    printf("%d\n", actions[hash_balise(&sep)](in, desc, &response));
  }
  return 0;
} //fin Main

