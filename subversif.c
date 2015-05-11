#include "peroraison.h"

char reponse_http[BUFSIZ];
Display *dpy;

int PopMuet(char* requete, int desc, pop* response)
{
  return -1;
}

liste_de_case *findCaseFromWindow(Window w, XTable *own)
{
  liste_de_ligne *q;
  liste_de_case *p;
  int i,j;

  for (q = own->premier, i=0; q ; q = q->next, i++)
    {
      for (p = q->premier,j=0; p ; p = p->next, j++)
  {
    if (w == p->fenetre)
      return p;
  }
    }
  return NULL;
}

liste_de_case *findCaseFromId(char *id, XTable *own)
{
  liste_de_ligne *q;
  liste_de_case *p;
  int n;

  id = index(id,'"');
  if (!id) return NULL;
  id++;
  n = index(id,'"')-id;

  for (q = own->premier; q ; q = q->next)
    {
      for (p = q->premier; p ; p = p->next)
  {
    if (p->id && (strlen(p->id) == n) && !strncmp(id, p->id, n))
        return p;
  }
    }

  return NULL;
}

int extractEnTete(char* enTete, char* source, char* destination){
  if(!strncmp(enTete, source, strlen(enTete))){
    source += strlen(enTete);
    char* tmp = index(source, '\n');
    *tmp = 0;
    strcpy(destination, source);
    return 0;
  }
  return -1;
}

void cleanPop(pop* response){
  response->nombreMessages = 0;
}

message* findById(int id, pop* response){
  if(response->nombreMessages == 0){
    printf("listeMessages BIEN VIDE!\n");
    return NULL;
  }

  message* tmp = response->listeMessages;
  int i;
  for(i = 0; i < response->nombreMessages-1; i++){
    if(tmp->id == id) return tmp;
    tmp = tmp->next;
  }
  printf("AUCUN ID NE CORRESPOND!!!\n\n\n"); 
  return NULL;
}

//ajoute un Message a la fin de la liste
void addMessage(pop* response, message* mex){
  //cas liste vide
  if(response->nombreMessages == 0){
    response->listeMessages = mex;
    response->nombreMessages = 1;
  }
  else{ //ajout message en fin de liste chainee
    message* tmp = response->listeMessages;

    int i;
    for(i = 0; i < response->nombreMessages-1; i++){
      tmp = tmp->next;
    }
    tmp->next = mex;
    response->nombreMessages++;
  }
}

void envoieServeur(char* requete, int desc){
  if (write(desc, requete, strlen(requete)) <=0)
    peroraison("write","erreur sur la socket", 5);
  fflush(NULL);
}

int reponsePositive(FILE* fdesc, char* firstLine){
  fgets(firstLine, LINELENGTH, fdesc);
  
  if (!strncmp("+OK", firstLine, strlen("+OK")))
    return 0; //reponse +OK
  else{   
    return -1; //reponse -ERR 
  }
}

int verifieSyntaxe(char* requete, char* controle){
  if (strncmp(controle, requete, strlen(controle))){
    printf("La Requete saisie n'est pas desservi par le protocole POP.\n");
    return -1;
  }
  printf("La requete %s est bien formulee'\n", controle);
  return 0;
}

int (*actions[27])(char* requete, int desc, pop* response) = {
  &PopMuet,   &PopMuet,   &PopMuet, // @ A B
  &PopMuet,   &PopMuet,   &PopMuet, // C D E
  &PopMuet,   &PopMuet,   &PopMuet, // F G H
  &PopMuet,   &PopMuet,   &PopMuet, // I J K
  &PopList,   &PopMuet,   &PopMuet, // L M N
  &PopMuet,   &PopPass,   &PopQuit, // O P Q
  &PopRetr,   &PopMuet,   &PopTop, // R S T
  &PopUser, &PopMuet,   &PopMuet, // U V W
  &PopMuet,   &PopMuet,   &PopMuet  // X Y Z
};


int main(int argc, char *argv[])
{
  int port;
  int desc;
  char in[LINELENGTH];

  //verification arguments
  if (argc < 4)
    peroraison("argc", "nombre d'arguments incorrect", 3);

  if (!sscanf(argv[2],"%d",&port)) 
    peroraison("sscanf", "argument incorrect", 4);

  if(strcmp(argv[3], "-t") && strcmp(argv[3], "-c") && strcmp(argv[3], "-g"))
    peroraison("option arg", "option argument missing", 5);

  //connexion au serveur
  printf("Connexion sur %s sur le port %d\n", argv[1],port);
  desc = InitConnexion(argv[1], port);

  //initialisation structure response
  pop response;
  response.nombreMessages = 0;
  response.listeMessages = NULL;

  if(!strcmp(argv[3], "-t")){ //MODE TEXTUEL
    while (fgets(in,LINELENGTH,stdin)){ 
      printf("REQUETE: %s\n", in);
      //fflush(NULL);

      //on divise le traitement par requetes...
      char sep = tolower(in[0]);
      printf("%d\n", actions[hash_balise(&sep)](in, desc, &response));
    }
  }else if(!strcmp(argv[3], "-c")){ //MODE CLICKABLE
    if(PopUser("USER moi\n", desc, &response) != 0)
      peroraison("PopUser", "User not recognized", 6);

    if(PopPass("PASS a\n", desc, &response) != 0)
      peroraison("PopPass", "Pass not recognized", 7);

    if(PopList("LIST\n", desc, &response) != 0)
      peroraison("PopList", "Message list cannot be retrieved", 8);

    Window fen;
    XEvent e;
    //char buf[BUFSIZ];
    XTable own;
    //int done;

    if ((dpy = XOpenDisplay(NULL)) == NULL)
      {fprintf(stderr, "%s: dispositif %s injoignable\n", argv[0],
         XDisplayName(NULL));
        exit(2);}
    
    own.premier = NULL;
    own.dernier = NULL;
    own.nb_tr= 0;
    own.nb_td= 0;
    own.width= 500;//100;
    own.height= 250;//50;

    fen = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 
             own.width + (MARGIN<<1), 
             own.height + (MARGIN<<1),
             0,
             BlackPixel(dpy,DefaultScreen(dpy)),
             WhitePixel(dpy,DefaultScreen(dpy)));

    XSelectInput(dpy,fen,KeyPressMask);
    
    createXTable(&own, &response);
    create_td_window(&own, fen);
    XMapWindow(dpy, fen);
    XMapSubwindows(dpy, fen);
    while (1) {
        XNextEvent(dpy, &e);
        switch (e.type)
        {
        case KeyPress: fKeyPress(); break;
        case LeaveNotify: {fLeave(&e.xcrossing, &own); break;}
        case EnterNotify: {fEnter(&e.xcrossing, &own); break;}
    //     case ButtonPress: {fButtonPress(&e.xbutton, &own); break;} // TME
        default: break;
        }
    }
    exit(0);


  }//fin mode clickable
 

  return 0;
} //fin Main

