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
  int count;
  int buffer_size = 1; char buffer[2]; buffer[1] = 0;
  KeySym keysym;
  XComposeStatus compose;
  count = XLookupString(event,buffer,buffer_size, &keysym, &compose);

  //Return Key
  if(keysym == XK_Return){
    if(sendLogin(user) != 0)
      restartLogin(user);
    else
      showListWindow();
  }//ASCII chars
  if ((keysym >= XK_space) && (keysym <= XK_asciitilde)){
    printf ("Ascii key:- ");
    updateLoginField(buffer, user);

    /*if (event->state & ShiftMask)
     printf("(Shift) %s\n", buffer);
    else if (event->state & LockMask)
     printf("(Caps Lock) %s\n", buffer);
    else if (event->state & ControlMask)
     printf("(Control) %c\n", 'a'+ buffer[0]-1) ;
    else printf("%s\n", buffer) ;*/
    } //SPECIAL CARACTERS 
    else if ((keysym >= XK_Shift_L) && (keysym <= XK_Hyper_R)){
      printf ("modifier key:- ");
      switch (keysym){
        case XK_Shift_L: printf("Left Shift\n"); break;
        case XK_Shift_R: printf("Right Shift\n");break;
        case XK_Control_L: printf("Left Control\n");break;
        case XK_Control_R: printf("Right Control\n"); break;
        case XK_Caps_Lock: printf("Caps Lock\n"); break;
        case XK_Shift_Lock: printf("Shift Lock\n");break;
        case XK_Meta_L: printf("Left Meta\n");  break;
        case XK_Meta_R: printf("Right Meta\n"); break;

      }
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
    } //F KEYS
    else if ((keysym >= XK_F1) && (keysym <= XK_F35)){
      printf ("function key %d pressed\n", (int)(keysym - XK_F1));
      if (buffer == NULL)
       printf("No matching string\n");
      else
       printf("matches <%s>\n",buffer);
    } //DELETE KEYS
    else if ((keysym == XK_BackSpace) || (keysym == XK_Delete)){
      delOneLoginField(buffer, user);

      printf("Delete\n");
    }

    else if ((keysym >= XK_KP_0) && (keysym <= XK_KP_9)){
      printf("Number pad key %d\n", (int)(keysym -  XK_KP_0));
      updateLoginField(buffer, user);
  }
  else if (keysym == XK_Break) {
    printf("closing dpy\n"); 
    XCloseDisplay(dpy); 
    exit (0);
  }else{
    printf("Not handled\n");
  }
}

void fButtonPress(XButtonEvent *e, XTable *own)
{
  loginFocus = e->window;
  /*if(loginFocus == loginWin[0])
    printf("Finestra USER\n");
  else if(loginFocus == loginWin[1])
    printf("Finestra PASSWD\n");*/
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
    sprintf(&affichage[0], "PASSWD: %s", user->password);
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
    sprintf(affichage, "PASSWD: %s", user->password);
    DISPLAYTEXT(loginWin[1], 50, 50, affichage);
  } 
}

int sendLogin(user* user){
  char request[30];
    //sending User-Name
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
  //XClearWindow(dpy, racine);
  //createXTable(&own, &response);
  //create_td_window(&own, racine);

}