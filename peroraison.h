#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>		/* pour EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/socket.h>		/* pour socket */
#include <netinet/in.h>		/* pour sockaddr_in et INET_ADDRSTRLEN */
#include <netdb.h>		/* pour gethostbyname */
#include <unistd.h>		/* pour read /write */
#include <fcntl.h>		/* pour open */
#include <arpa/inet.h>		/* pour inet_aton */
#include <time.h>		/* clair */
#include <ctype.h> /* pour isalpha et isdigit */
#include <errno.h> /* pour perror et errno */
#include "expat.h"

#define hash_balise(b) ((('a' <= *(b)) && (*(b) <= 'z')) ? (*(b)-'a'+1) : 0)
#define PROXY proxy
#define PROXY_PORT 1080
#define LINELENGTH 1024
#define TRUE 1
#define FALSE 0

typedef struct{
  int id;
  char *emetteur;
  char *date;
  char *contenu;
  char *name;
} message;

typedef struct {
  int nombreMessages;
  message* listeMessages;
} pop;

extern void envoieServeur(char* requete, int desc);
extern int reponsePositive(FILE* fdesc, char* firstLine);
extern int verifieSyntaxe(char* requete, char* controle);
//void printSvnStruct(svn *own);
extern char reponse_http[BUFSIZ];
extern int PopMuet(char* requete, int desc, pop* response);
extern int (*actions[27])(char* requete, int desc, pop* response);
extern int main(int argc, char *argv[]);
extern char argv0[128];
extern void peroraison (char *f, char *m, int n);
extern int InitConnexion(char *serveur, int port);
//extern void update_entries(svn *svnentries);
//extern char *SvnDelete(int argc, char *argv[], svn *own);
extern int PopQuit(char* requete, int desc, pop* response);
extern int PopPass(char* requete, int desc, pop* response);
extern int PopUser(char* requete, int desc, pop* response);
extern int PopList(char* requete, int desc, pop* response);

//extern char *SvnCommit(int argc, char *argv[], svn *own);
//extern char *SvnUpdate(int argc, char *argv[], svn *own);
extern void AnalyseEntetes(FILE *fdesc, int *n);
