#include "peroraison.h"

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




