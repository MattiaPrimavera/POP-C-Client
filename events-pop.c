#include "pop.h"

void fExpose(XExposeEvent *e, user* user)
{
  //DRAWING ON RETR WINDOWS IF CREATED
  //if(retrWin != 0)
  //  XDrawString(dpy, retrWin, gc, 150, 100, "CIAOOOOOOOOO", strlen("CIAOOOOOOOOO"));

  int i;
  for(i=0; i<10; i++){
    if(retrWinList[i].mexId >= 0){ //if one RETRWINDOW is opened
      message* mex = findById(i, &response);
      if(mex != NULL){
        XDrawString(dpy, retrWinList[i].main, gc, 20, 100, mex->entetes, strlen(mex->entetes));
      }
    }
  }

  //DRAWING LOGGIN WINDOW
  DISPLAYTEXT(loginWin[0], 50, 50, "USER: ");
  DISPLAYTEXT(loginWin[1], 50, 50, "PASSWD: ");
}

void fEnter(XCrossingEvent *e, GridWinInfo *own){ }

void fLeave(XCrossingEvent *e, GridWinInfo *own){ }

void fKeyPress (XKeyEvent *event, user* user)
{
  static int userOk = FALSE, passwdOK = FALSE;
  int buffer_size = 1; char buffer[2]; buffer[1] = 0;
  KeySym keysym;
  XComposeStatus compose;
  XLookupString(event,buffer,buffer_size, &keysym, &compose);

  //Return Key
  if(keysym == XK_Return){
    printf("retour chariot\n");
    if((!passwdOK && userOk) || (passwdOK && !userOk)){
      printf("Connexion sur %s sur le port %d\n", server.serverAddress, server.port);
      desc = InitConnexion(server.serverAddress, server.port);  
      if(sendLogin(user) != 0){
        restartLogin(user);
        passwdOK = FALSE; userOk = FALSE;
      }
      else{
        showListWindow();
      }
    }
    if(loginFocus == loginWin[0]){
      printf("Setting User True\n");
      userOk = TRUE; loginFocus = loginWin[1];
    }
    else if(loginFocus == loginWin[1]){
      printf("Setting Pass True\n");
      passwdOK = TRUE; loginFocus = loginWin[0];
    }
  }//ASCII chars
  else if ((keysym >= XK_space) && (keysym <= XK_asciitilde)){
    printf ("Ascii key:- ");
    updateLoginField(buffer, user);
  } //ARROW KEYS
  else if ((keysym >= XK_Left) && (keysym <= XK_Down)){
    printf("Arrow Key:-");
    switch(keysym){
      case XK_Up:
      if(loginFocus != loginWin[0]) loginFocus = loginWin[0]; 
      printf("Up\n");
      break;
      case XK_Down: 
      if(loginFocus != loginWin[1]) loginFocus = loginWin[1];
      printf("Down\n");
      break; 
      default:
        break;
    }
  } //DELETE KEYS
  else if ((keysym == XK_BackSpace) || (keysym == XK_Delete)){
    delOneLoginField(buffer, user);
  } //NUMBER PAD KEYS
  else if ((keysym >= XK_KP_0) && (keysym <= XK_KP_9)){
    updateLoginField(buffer, user);
  } //BREAK CARACTER
  else if (keysym == XK_Break) {
    XCloseDisplay(dpy); 
    exit (0);
  }else{
    printf("Not handled\n");
  }
}

void fButtonPress(XButtonEvent *e, char* option)
{
  //CHANGING THE FOCUS (LOGGIN WINDOW)
  loginFocus = e->window;
  
  //QUIT BUTTON EVENT
  if(e->window == filles[own.nb_tr * own.nb_td]) PopQuit("QUIT\n", desc, &response);
  
  //ID CLICK EVENT -> RETR REQUEST 
  int i;
  for(i=1; i<own.nb_tr-1; i++){
    if(e->window == filles[i*3]){
      char requete[20];
      sprintf(requete, "RETR %d\n", i);
      PopRetr(requete, desc, &response);
      if(!strcmp(option, "-g")){
        printf("\nCREATION RETR WINDOWWWWWWWWWWWWWWWWWWWWWW\n");
        createRetrWindow(i);
      }
    }
  }//end for
}

void updateLoginField(char* buffer, user* user){
  char affichage[20];
  if(loginFocus == loginWin[0]){
    if(strlen(user->name) > 15) return;
    strcat(user->name, buffer);
    sprintf(&affichage[0], "USER: %s", user->name);
    DISPLAYTEXT(loginWin[0], 50, 50, affichage);
  }
  else{
    if(strlen(user->password) > 15) return;
    strcat(user->password, buffer);
    sprintf(&affichage[0], "PASSWD: ");
    int i;
    for(i=0; i<strlen(user->password); i++) strcat(affichage, "*");
      DISPLAYTEXT(loginWin[1], 50, 50, affichage);
  }
}

void delOneLoginField(char* buffer, user* user){
  char affichage[20];
  if(loginFocus == loginWin[0]){
    user->name[strlen(user->name)-1] = 0;
    printf("EXPOSE --> logWin[0]\n");
    XClearWindow(dpy, loginWin[0]);
    sprintf(affichage, "USER: %s", user->name);
    DISPLAYTEXT(loginWin[0], 50, 50, affichage);
  }
  else{
    user->password[strlen(user->password)-1] = 0;
    printf("EXPOSE --> logWin[1]\n");
    XClearWindow(dpy, loginWin[1]);
    sprintf(affichage, "PASSWD: ");
    int i;
    for(i=0; i<strlen(user->password); i++) strcat(affichage, "*");
      DISPLAYTEXT(loginWin[1], 50, 50, affichage);
  } 
}

int sendLogin(user* user){
  char request[30];
  sprintf(request, "USER %s\n", user->name);
  printf("RICHIESTA DA INVIARE:%s", request);
  if(PopUser(request, desc, &response) != 0)
    return -1;
  sprintf(request, "PASS %s\n", user->password);
  if(PopPass(request, desc, &response) != 0)
    return -1;
  if(PopList("LIST\n", desc, &response) != 0)
    return -1;
  //TOP request for each message retrieved
  //if(response.nombreMessages == 0) return -1;
  //PopTop("TOP 1 0\n", desc, &response);

  
  int i;
  for(i=0; i<response.nombreMessages-1; i++){
    sprintf(request, "TOP %d 0\n", i);
    PopTop(request, desc, &response);
    //if(PopTop(request, desc, &response) != 0)
    //  return -1;
  }
  
  return 0;
}

void restartLogin(user* user){
  XUnmapWindow(dpy, loginWin[0]);
  XUnmapWindow(dpy, loginWin[1]);

  DISPLAYTEXT(loginWin[0], 50, 50, "USER: ");
  bzero(user->name, 20);
  DISPLAYTEXT(loginWin[1], 50, 50, "PASSWD: ");  
  bzero(user->password, 20);

  XMapWindow(dpy, loginWin[0]);
  XMapWindow(dpy, loginWin[1]);
}

void showListWindow(){

  XUnmapWindow(dpy, loginWin[0]);
  XUnmapWindow(dpy, loginWin[1]);
  XClearWindow(dpy, racine);
  createListWindow();
  XMapWindow(dpy, racine);
}