#include "peroraison.h"

void AnalyseEntetes(FILE *fdesc, int *n)
{
  char buf[LINELENGTH];


  while (fgets(buf, LINELENGTH, fdesc))
  {    
    char *ptr;
    if (strlen(buf) <= 2) break;
    write(STDOUT_FILENO, buf, strlen(buf));
    fflush(NULL);

    //treatement reponse POSITIVE    
    if (!strncmp(buf, "+OK", strlen("+OK"))){
      printf("REPONSE POSITIVE!\n");
      break;
    }

    //treatement reponse NEGATIVE    
    if (!strncmp(buf, "-ERR", strlen("-ERR"))){
      printf("REPONSE NEGATIVE!\n");
      break;
    }

    /*if (sscanf(buf, "Content-Length: %d", n)) {
      continue;
    }
    if (stype || strncmp(buf, "Content-Type: ", strlen("Content-Type: ")))
      continue;

    ptr = strchr (buf+strlen("Content-Type: "), '/');
    if (!ptr) continue;
    ptr++;
    stype = strsep (&ptr, "; \r\n");

  // dans le cas "mixed",  on garde ce qui suit (cf q5).

    if (!strncmp(stype, "mixed", strlen("mixed"))) {
      ptr[-1] = ';' ;
      strtok_r (0, "; \r\n", &ptr) ;
      return strdup(stype) ;
    }*/

    //stype = strdup( stype );
  }
  // Il vaudrait renvoyer stype toujours, pour faire Free si non vide.
  //return stype ? stype : "html";
}
