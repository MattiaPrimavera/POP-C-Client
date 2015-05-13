#include "peroraison.h"

void fExpose(XExposeEvent *e, user* user)
{
  DISPLAYTEXT(loginWin[0], 50, 50, "USER: ");
  DISPLAYTEXT(loginWin[1], 50, 50, "PASSWD: ");
}

void fEnter(XCrossingEvent *e, XTable *own){ }

void fLeave(XCrossingEvent *e, XTable *own){ }

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
      if(sendLogin(user) != 0){
        restartLogin(user);
        passwdOK = FALSE; userOk = FALSE;
      }
      else
        showListWindow();
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
      case XK_Left: printf("Left\n");break;
      case XK_Up:
      if(loginFocus != loginWin[0]) loginFocus = loginWin[0]; 
      printf("Up\n");
      break;
      case XK_Right: printf("Right\n");break;
      case XK_Down: 
      if(loginFocus != loginWin[1]) loginFocus = loginWin[1];
      printf("Down\n");
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

void fButtonPress(XButtonEvent *e, XTable *own)
{
  loginFocus = e->window;
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
  //createXTable();
  //create_td_window(racine);
  XMapWindow(dpy, racine);
}