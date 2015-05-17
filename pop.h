#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
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
#include <ctype.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h> 
#include <X11/keysym.h>

#define hash_balise(b) ((('a' <= *(b)) && (*(b) <= 'z')) ? (*(b)-'a'+1) : 0)
#define LINELENGTH 1024
#define TRUE 1
#define FALSE 0

typedef struct message message;
struct message{
  int id;
  int taille;
  char *emetteur;
  char *date;
  char* entetes;
  char *corps;
  char* contentType;
  message* next;
};

typedef struct {
  int nombreMessages;
  message* listeMessages;
} pop;

typedef struct{
  char* serverAddress;
  int port;
} server_info;

//textuel global variables
extern int desc;
extern server_info server;
extern pop response;

extern void textuelMain(int argc, char** argv);

//textual utilities
extern void cleanPop(pop* response);
extern int extractEnTete(char* enTete, char* source, char* destination);
extern message* findById(int id, pop* response);
extern void addMessage(pop* response, message* mex);
extern void envoieServeur(char* requete, int desc);
extern int reponsePositive(FILE* fdesc, char* firstLine);
extern int verifieSyntaxe(char* requete, char* controle);
extern int (*actions[27])(char* requete, int desc, pop* response);
extern int main(int argc, char *argv[]);
extern void peroraison (char *f, char *m, int n);
extern int InitConnexion(char *serveur, int port);
extern void AnalyseEntetes(char* requete, int mexId, FILE *fdesc, pop* response);
extern void sauvegardeMessage(char* contentType, int mexId, char* entetes, char* corps);
extern char* sauvegardeEnTetes(char* entetes, FILE* fdesc, message* mex);
extern void sauvegardeCorps(char* reponse, FILE* fdesc);
extern char* isMimeSimple(char* contentType);
extern char* extractExtension(char* type, char* source);
extern void SauveContenu(char* extension, int mexId, char* source, char* mode);

//requests functions
extern int PopMuet(char* requete, int desc, pop* response);
extern int PopQuit(char* requete, int desc, pop* response);
extern int PopPass(char* requete, int desc, pop* response);
extern int PopUser(char* requete, int desc, pop* response);
extern int PopList(char* requete, int desc, pop* response);
extern int PopTop(char* requete, int desc, pop* response);
extern int PopRetr(char* requete, int desc, pop* response);


//GRAPHIC PART
#define DISPLAYTEXT(win, x, y, text) \
 XDrawString(dpy, win, gc, x, y, text, strlen(text));

#define MARGIN 10
#define BORDER 2
#define nb_LIGNES 10

typedef struct{
  char name[20];
  char password[20];
} user;

typedef struct {
  int nb_tr;
  int nb_td;
  int width;
  int height;
} GridWinInfo;

typedef struct{
  Window main;
  Window scroll;
  int mexId;
} RetrWin;

extern Display *dpy;
extern Window racine, filles[nb_LIGNES*3], loginWin[2], retrWinSub, retrWin;
extern GC gc;
extern RetrWin retrWinList[10];
extern XFontStruct *font;
extern Window loginFocus;
extern   Visual *visual; 
extern  int screen;
extern  int depth; 
extern       XSetWindowAttributes attributes; 
extern XFontStruct *fontinfo; 
extern GridWinInfo own;

extern void graphicalMain(int argc, char** argv);


//displaying windows
extern void createRetrWindow();
extern void createXLoginTable();
extern void createLoginWindow(Window fen);
extern void createMainWindow();
extern void createListWindow();

//managing events
extern void fEnter(XCrossingEvent *e, GridWinInfo *own);
extern void fLeave(XCrossingEvent *e, GridWinInfo *own);
extern void fKeyPress(XKeyEvent *event, user* user);
extern void fButtonPress(XButtonEvent *e, char* option);
extern void fExpose(XExposeEvent *e, user* user);

//utilities
extern void showListWindow();

extern void decoratingListWindow();
extern void updateLoginField(char* buffer, user* user);
extern void delOneLoginField(char* buffer, user* user);
extern int sendLogin(user* user);
extern void restartLogin();
