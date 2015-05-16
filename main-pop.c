#include "peroraison.h"


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
