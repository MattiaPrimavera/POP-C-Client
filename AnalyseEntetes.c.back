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

  while (fgets(reponse, LINELENGTH, fdesc)){    
    //condition d'arret
    if(!strncmp(".", reponse, strlen("."))){
      printf("\nfin entetes ...\n");
      break;
    }

    printf("%s", reponse);

    //sauvegarde en-tete From
    if(!from){
      if(!extractEnTete("From: ", reponse, mex->emetteur))
        from=TRUE;
      //if(sscanf(reponse, "From: %s\n", mex->emetteur))
      //  from=TRUE;
    }

    //sauvegarde en-tete Date
    if(!date){
      if(!extractEnTete("Date: ", reponse, mex->date))
        date=TRUE;

      //if(sscanf(reponse, "Date: %s\n", mex->date))
        //date=TRUE;
    }
  }//fin while
  printf("En-tetes trouvees:\nFrom: %s\nDate: %s\n", mex->emetteur, mex->date);

}
