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
#include <ctype.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

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
  char *contenu;
  message* next;
};

typedef struct {
  int nombreMessages;
  message* listeMessages;
} pop;


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
extern void AnalyseEntetes(int mexId, FILE *fdesc, pop* response);

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

//extern int desc;
extern Display *dpy;
extern Window racine, filles[nb_LIGNES];
extern GC gc;

typedef struct{
  char name[15];
  char password[15];
} user;

typedef struct desc_case {
  int colspan;
  int rowspan;
  char *onmouseover;
  char *onmouseout;
  XColor bgcolor;
  char *id;
  Window fenetre;
  struct desc_case * next;
} liste_de_case;

typedef struct desc_ligne {
  struct desc_case * premier;
  struct desc_case * dernier;
  struct desc_ligne * next;
} liste_de_ligne;

typedef struct {
  int nb_tr;
  int nb_td;
  int width;
  int height;
  liste_de_ligne *premier;
  liste_de_ligne *dernier;
} XTable;

//displaying windows
extern void createXLoginTable(void *data, user* admin);
extern void createXTable(void *data, pop* response);
extern void createTableLine(void *data);
extern void createTableCell(void *data);
extern void add_line(XTable *t, liste_de_ligne *r);
extern void add_cell(XTable *t, liste_de_case *r);
extern void create_td_window(XTable *own, Window fen);
//extern liste_de_case *findCaseFromWindow(Window w, XTable *own);
//extern liste_de_case *findCaseFromId(char *id, XTable *own);

//managing events
extern void fEnter(XCrossingEvent *e, XTable *own);
extern void fLeave(XCrossingEvent *e, XTable *own);
extern void fKeyPress(XKeyEvent *e);
extern void fButtonPress(XButtonEvent *e, XTable *own);


extern void creerGC();
