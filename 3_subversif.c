#include "peroraison.h"

char *SvnCommit(int argc, char *argv[], svn *own)
{
   printf("dans commit \n\n");
   entries *p;
   int n, desc;
   char buffet[1024];

   for (p=own->contenu; p; p=p->next)
      if (!strcmp(p->name, argv[2])) break;
   if (!p) {
     fprintf(stderr, "Fichier %s inconnu. Utiliser la commande add\n", argv[2]);
      exit(-1);
   }

   desc = InitConnexion(own->server, 5556);

   printf("Apres init connection\n");
   // cas modifcation ou ajout
   if ( !p->schedule || !strcmp(p->schedule, "add") ) { 
      //ouverture descripteur fichier
      int f;
      f = open(argv[2],O_RDONLY);
      if (f < 0)  {
	     fprintf(stderr, "Fichier %s  introuvable\n", argv[2]);
	     exit(-1);
      }
      printf("avant envoie requete put ...\n");
      printSvnStruct(own);
      //envoie requete PUT 
      write(desc, "PUT ", 4);
      write(desc, own->serverpath, strlen(own->serverpath));
      write(desc, argv[2], strlen(argv[2]));
      write(desc, "\nHost: ", strlen("\nHost: "));
      write(desc, own->server, strlen(own->server));
      write(desc, "\nIf-Not-Modified-Since ", strlen("\nIf-Not-Modified-Since "));
      write(desc, "\n", 1);
      if(p->committed_date) write(desc, p->committed_date, strlen(p->committed_date));
      write(desc, "\n\n", 2);

      printf("apres envoie du fichier\n\n");
      //envoie du fichier dans une ligne 
      while((n=read(f,buffet, 1024)) != EOF) write(desc,buffet,n);
      close(f);


   } //cas cancelation 
   else if ( !strcmp(p->schedule, "delete") ) { //suppression
      //envoie requete DELETE
      write(desc, "DELETE /", 8);
      write (desc, own->serverpath, strlen(own->serverpath));
      write(desc,"/", 1);
      write(desc, argv[2], strlen(argv[2]));
      write(desc, "\nHost: ", strlen("\nHost: "));
      write(desc, own->server, strlen(own->server));
      write(desc, "\n\n", 2);
   } else {
     peroraison("SvnCommit", "Erreur dans svn/entires\n", -1);
   }

   n = read(desc, reponse_http, BUFSIZ-1);
   if (n <= 0) return NULL;
   reponse_http[n] = 0;
   return reponse_http;
}

