#include "peroraison.h"

void AnalyseEntetes(char* requete, int mexId, FILE *fdesc, pop* response)
{
  char entetes[LINELENGTH];
  
  //récuperation ou création du message
  message *mex;
  if((mex = findById(mexId, response)) == NULL){
    printf("Message not Found ... Creating one ...\n");
    mex = (message*)malloc(sizeof(message));
    mex->emetteur = (char*)malloc(150*sizeof(char));
    mex->date = (char*)malloc(150*sizeof(char));
    addMessage(response, mex);
  }
  else{ //message déjà dans listeMessages
    mex->emetteur = (char*)malloc(150*sizeof(char));
    mex->date = (char*)malloc(150*sizeof(char));
    printf("LE MESSAGE A BIEN ETE' TROUVE' \n");
  }

  char* contentType = sauvegardeEnTetes(entetes, fdesc, mex);
  printf("En-tetes trouvees:\nFrom: %s\nDate: %s\n", mex->emetteur, mex->date);
  printf("Content-Type found :%s\n", contentType);
  if(!strncmp(requete, "RETR", strlen("RETR"))){
    char corps[LINELENGTH*4];
    sauvegardeCorps(corps, fdesc);
    printf("CORPS:\n%sFINCORPS\n", corps);
  }
  printf("ENTETES:\n%sFIN ENTETES\n", entetes);

}

char* sauvegardeEnTetes(char* entetes, FILE* fdesc, message* mex){
  int from = FALSE, date = FALSE, content=FALSE;
  printf("in analyse entetes ... \n");
  char* contentType = (char*)malloc(150*sizeof(char));

  char* reponse = entetes;
  //Analyse En-tetes
  while (fgets(reponse, LINELENGTH, fdesc)){    
    //conditions d'arret
    if(!strncmp(".", reponse, strlen("."))){
      printf("\nfin entetes ...\n");
      break;
    }//le corps du message va suivre 
    if(!strncmp("\n", reponse, strlen("\n"))){
      printf("\nfin entetes ...\n");
      break;
    }
    //printf("%s", reponse);

    //sauvegarde en-tetes From et Date
    if(!from){
      if(!extractEnTete("From: ", reponse, mex->emetteur))
        from=TRUE;
    }
    if(!date){
      if(!extractEnTete("Date: ", reponse, mex->date))
        date=TRUE;
    }
    if(!content){
      if(!extractEnTete("Content-Type: ", reponse, contentType))
        content=TRUE;
    }
    reponse = reponse + strlen(reponse);
  }//fin while
  if(content) return contentType;
  else return NULL;
}

void sauvegardeCorps(char* reponse, FILE* fdesc){
  //sauvegarde du corps
  char* tmp = reponse;
  while(fgets(tmp, LINELENGTH, fdesc)){
    printf("tmp:%s", tmp);
    if(!strncmp(".", tmp, strlen("."))){
      printf("\nfin CORPS ...\n");
      break;
    }
    tmp = tmp + strlen(tmp);
  }//fin while
}

void sauvegardeMessage(char* contentType, int mexId, char* entetes, char* corps){
  if(!contentType){ //CONTENT-TYPE ABSENT

  }
  else if(!isMimeSimple(contentType)){

  }
  else if(!isMimeMultipart(contentType)){
    
  }
}

void SauveContenu(char *stype, FILE *fdesc, int size)
{
  char buf[FILENAME_MAX];
  size_t n;
  int k;
  FILE *fd;

  sprintf(buf, "/tmp/%d.%s", (int)time(NULL), stype);
  fd = fopen(buf, "w");
  if (fd == NULL)
    peroraison("fopen", "Impossible d'enregistrer le fichier", 2);
  printf("Creation du fichier %s\n", buf);
  while (1) {
      k = (size > FILENAME_MAX) ? size : FILENAME_MAX;
      n = fread(buf, sizeof(char), k, fdesc);
      if (!n) break;
      fwrite(buf, sizeof(char), n, fd);
      size -= n;
      if (!size) break;
      printf("Encore  %d char\n", size);
  }
  fclose(fd);
}

char* isMimeSimple(char* contentType){
  return NULL;
}

char* isMimeMultipart(char* contentType){
  return NULL;
}