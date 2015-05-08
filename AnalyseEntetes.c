#include "peroraison.h"

void AnalyseEntetes(int mexId, FILE *fdesc, pop* response)
{
  char reponse[LINELENGTH];
  int from = FALSE, date = FALSE;

  printf("in analyse entetes ... \n");
  
  //ERRORE NELLA FUNZIONE findByID non trova un cazzo, sembra che list non memorizzi l'id

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
  }//fin while
  printf("En-tetes trouvees:\nFrom: %s\nDate: %s\n", mex->emetteur, mex->date);

}
