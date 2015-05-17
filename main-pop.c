#include "pop.h"

void peroraison(char *f, char *m, int n)
{
  perror(m);
  exit(n);
}

int InitConnexion(char *serveur, int port)
{
 int maSocket;
 struct sockaddr_in const *sin;
 char ascii_buffer[LINELENGTH];
 char sport[255];
 char *cause = NULL;

 struct addrinfo addrHints, *addrResult; // adresses desirees / recuperees 
 struct addrinfo *addrTmp;    //adresse temporaire
  
 int i;
 int error;
 sprintf(sport,"%i",port);  //conversion du no de port en chaine de char
 memset(&addrHints, 0, sizeof(addrHints));     // raz 
 addrHints.ai_family = AF_INET;   //famille  TCP/IP ipv4
 addrHints.ai_socktype = SOCK_STREAM; // avoir une socket de type stream
 
 error = getaddrinfo(serveur, sport, &addrHints, &addrResult);
 if (error) {
    printf("Client Erreur %d getaddrinfo error for host %s %s:\n", error,serveur, sport); 
    printf("\t%s\n",gai_strerror(error));
    exit (EXIT_FAILURE);
 }
 i=0;
 //recherche toute les IP du serveur
 for (addrTmp = addrResult; addrTmp; addrTmp = addrTmp->ai_next) {
   i++;
   printf("  socket no%d:\n\tdomain:%d type:%d protocole:%d\n",i,
    addrTmp->ai_family, addrTmp->ai_socktype,addrTmp->ai_protocol);
   //ai_proto=6 pour TCp alors  AFINET pour ipv4 par default
   sin = (void *)addrTmp->ai_addr;
   if (inet_ntop(AF_INET, &sin->sin_addr, ascii_buffer, sizeof(ascii_buffer)) == NULL)
     printf(" inet_ntop : Echec \n");
   else
     printf("\t%s\n", ascii_buffer);
 }
 //connection a la premiere socket
 maSocket = -1;
 for (addrTmp = addrResult; addrTmp; addrTmp = addrTmp->ai_next) {
   maSocket = socket(addrTmp->ai_family, addrTmp->ai_socktype,addrTmp->ai_protocol);
   if (maSocket < 0) {
     cause = "socket";
     continue;}
   if (connect(maSocket, addrTmp->ai_addr, addrTmp->ai_addrlen) < 0) {
     cause = "connect";
     close(maSocket);
     maSocket = -1;
     continue;
   }
   break;
  }  /* okay we got one */
 
 if (maSocket < 0) {/*NOTREACHED*/
   printf("Erreur %s", cause);
   peroraison("socket ou connect","erreur de connection ou de socket", 1);}

 return maSocket;
}

server_info server;
int main(int argc, char *argv[])
{

  //VERIFICATION ARGUMENTS
  if (argc < 4)
    peroraison("argc", "nombre d'arguments incorrect", 3);

  if (!sscanf(argv[2],"%d",&(server.port))) 
    peroraison("sscanf", "argument incorrect", 4);

  if(strcmp(argv[3], "-t") && strcmp(argv[3], "-c") && strcmp(argv[3], "-g"))
    peroraison("option arg", "option argument missing", 5);
  server.serverAddress = argv[1];

  //INITIALIZATION STRUCTURE REPONSE
  response.nombreMessages = 0;
  response.listeMessages = NULL;

  if(!strcmp(argv[3], "-t")){ //MODE TEXTUEL
    textuelMain(argc, argv);
  }else if(!strcmp(argv[3], "-c")){ //MODE CLICKABLE
    graphicalMain(argc, argv);
  }else if(!strcmp(argv[3], "-g")){ //MODE GRAPHIQUE
    graphicalMain(argc, argv);
  }

  return 0;
} //fin Main

