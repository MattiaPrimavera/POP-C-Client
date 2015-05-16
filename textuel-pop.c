#include "peroraison.h"

//connexion
int desc;
pop response;

int extractEnTete(char* enTete, char* source, char* destination){
  if(!strncmp(enTete, source, strlen(enTete))){
    source += strlen(enTete);
    char* tmp = index(source, '\n');
    *tmp = 0;
    strcpy(destination, source);
    return 0;
  }
  return -1;
}

void cleanPop(pop* response){
  response->nombreMessages = 0;
}

message* findById(int id, pop* response){
  if(response->nombreMessages == 0){
    printf("listeMessages BIEN VIDE!\n");
    return NULL;
  }

  message* tmp = response->listeMessages;
  int i;
  for(i = 0; i < response->nombreMessages-1; i++){
    if(tmp->id == id) return tmp;
    tmp = tmp->next;
  }
  printf("AUCUN ID NE CORRESPOND!!!\n\n\n"); 
  return NULL;
}

//ajoute un Message a la fin de la liste
void addMessage(pop* response, message* mex){
  //cas liste vide
  if(response->nombreMessages == 0){
    response->listeMessages = mex;
    response->nombreMessages = 1;
  }
  else{ //ajout message en fin de liste chainee
    message* tmp = response->listeMessages;

    int i;
    for(i = 0; i < response->nombreMessages-1; i++){
      tmp = tmp->next;
    }
    tmp->next = mex;
    response->nombreMessages++;
  }
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
  &PopList,   &PopMuet,   &PopMuet, // L M N
  &PopMuet,   &PopPass,   &PopQuit, // O P Q
  &PopRetr,   &PopMuet,   &PopTop, // R S T
  &PopUser, &PopMuet,   &PopMuet, // U V W
  &PopMuet,   &PopMuet,   &PopMuet  // X Y Z
};

void textuelMain(int argc, char** argv){
	char in[LINELENGTH];
  	
  	//CONNEXION AU SERVEUR
  	printf("Connexion sur %s sur le port %d\n", server.serverAddress , server.port);
  	desc = InitConnexion(server.serverAddress, server.port);    

    while (fgets(in,LINELENGTH,stdin)){ 
      //printf("REQUETE: %s\n", in);
      char sep = tolower(in[0]);
      printf("%d\n", actions[hash_balise(&sep)](in, desc, &response));
    }
}