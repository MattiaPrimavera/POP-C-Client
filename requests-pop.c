#include "pop.h"

int PopUser(char* requete, int desc, pop* response)
{
   printf("in PopUser function\n");
   if(verifieSyntaxe(requete, "USER") != 0) return -1;

   envoieServeur(requete, desc);

   //lecture Reponse
   FILE* fdesc = fdopen(desc, "r");
   char reponse[LINELENGTH];

   printf("Preparation Lecture Reponse\n");
   fflush(NULL);
   if(reponsePositive(fdesc, &reponse[0]) != 0){
      printf("REPONSE NEG:%s\n", reponse);
      return -1;
   }
   printf("REPONSE:%s\n", reponse);

   return 0;
}  

int PopPass(char* requete, int desc, pop* response)
{
   printf("in PopPass function\n");
   if(verifieSyntaxe(requete, "PASS") != 0) return -1;

   envoieServeur(requete, desc);

   //lecture Reponse
   FILE* fdesc = fdopen(desc, "r");
   char reponse[LINELENGTH];
   printf("%s\n", reponse);

   if(reponsePositive(fdesc, reponse) != 0) return -1;
   
   return 0;
}  

int PopQuit(char* requete, int desc, pop* response)
{
   printf("in PopQuit function\n");
   if(verifieSyntaxe(requete, "QUIT") != 0) return -1;

   envoieServeur(requete, desc);

   //lecture Reponse
   FILE* fdesc = fdopen(desc, "r");
   char reponse[LINELENGTH];

   if(reponsePositive(fdesc, reponse) != 0) return -1;
   printf("reponse:%s\n", reponse);

   //correcte sortie du programme
   printf("Vous avez choisi de Quitter l'application...\n");
   printf("Merci pour avoir utiliser notre Client POP.\n");
   exit (EXIT_SUCCESS);
}  

int PopMuet(char* requete, int desc, pop* response){
  return -1;
}

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
   AnalyseEntetes(requete, mexId, fdesc, response);

    //sortie avec erreur
   return 0;
}

int PopList(char* requete, int desc, pop* response)
{
   printf("in PopList function\n");
   if(verifieSyntaxe(requete, "LIST") != 0) return -1;

   envoieServeur(requete, desc);

   //lecture 1ere ligne
   FILE* fdesc = fdopen(desc, "r");
   char reponse[LINELENGTH];
   printf("%s", reponse);
   if(reponsePositive(fdesc, reponse) != 0) return -1;

   cleanPop(response);
   //lecture corps reponse
   while (fgets(reponse, LINELENGTH, fdesc)){
      printf("%s", reponse);

      //condition d'arret
      if(!strncmp(".", reponse, strlen("."))){
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


/*int PopRetr(char* requete, int desc, pop* response)
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
   char* contentType = AnalyseEntetes(mexId, fdesc, response);
   printf("contentType found: %s\n", contentType);
   sauvegardeMessage(contentType, int mexId);

   //sauvegarde du corps
   char* tmp = reponse;
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
}*/

