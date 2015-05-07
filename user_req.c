#include "peroraison.h"

int PopUser(char* requete, int desc, pop* response)
{
   printf("in PopUser function\n");
   if(verifieSyntaxe(requete, "USER") != 0) return -1;

   envoieServeur(requete, desc);

   //lecture Reponse
   FILE* fdesc = fdopen(desc, "r");
   char reponse[LINELENGTH];
   printf("%s\n", reponse);

   if(reponsePositive(fdesc, reponse) != 0) return -1;
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
   printf("%s\n", reponse);

   if(reponsePositive(fdesc, reponse) != 0) return -1;

   //correcte sortie du programme
   printf("Vous avez choisi de Quitter l'application...\n");
   printf("Merci pour avoir utiliser notre Client POP.\n");
   exit (EXIT_SUCCESS);
}  

/* 
   fdesc = fdopen(desc,"r");

 fclose(fdesc);
  exit (EXIT_SUCCESS);
   AnalyseEntetes(fdesc, &n);
   printf("SORTIE D'ANALYSE ENTETES\n");

    type = AnalyseEntetes(fdesc, &n);
    printf("recu document de type %s longeur %d\n", type, n);
    SauveContenu(type,fdesc);*/


   /*entries *p;
   int n, desc;
   char buffet[1024];

   for (p=own->contenu; p; p=p->next)
      if (!strcmp(p->name, argv[2])) break;
   if (!p) {
     fprintf(stderr, "Fichier %s inconnu. Utiliser la commande add\n", argv[2]);
      exit(-1);
   }

   desc = InitConnexion(own->server, 80);
   
   //envoie requete GET 
   write(desc, "GET ", 4);
   write(desc, own->serverpath, strlen(own->serverpath));
   write(desc, argv[2], strlen(argv[2]));
   write(desc, "\nHost: ", strlen("\nHost: "));
   write(desc, own->server, strlen(own->server));
   if(p->committed_date){ //si on a pas de committed date -> nouveau fichier
      write(desc, "\nIf-Modified-Since ", strlen("\nIf-Modified-Since "));
      write(desc, p->committed_date, strlen(p->committed_date));
      write(desc, "\n", 1);
   }
   write(desc, "\n\n", 2);

   //reception réponse (1st line)   
   FILE* fd = fdopen(desc, "r");
   if(f<0) perror("erreur ouverture flow\n");
   
   //interpretation premier ligne 
   fgets(buffet, LINELENGTH, fd);
   if(!strcmp(buffet, "HTTP/1.1 304 Not-Modified\n")){
      printf("304: Vous avez déjà la dernière version du fichier\n");
   }
   else{ //Analyse en-tetes
      long fileSize = AnalyseEntetes(f, p);
      //écriture dans le fichier
      FILE* outputFile = fopen(p->name, "w+"); 
      char caract = 0;
      while((caract = fgetc(f)) != EOF) fputc(caract, outputFile);
      update_entries(own); //mise a jour fichier xml
   }
   return NULL;  */


