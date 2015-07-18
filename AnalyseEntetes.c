#include "pop.h"

void AnalyseEntetes(char* requete, int mexId, FILE *fdesc, pop* response)
{
    //char entetes[LINELENGTH];

    //récuperation ou création du message
    message *mex;

    if ((mex = findById(mexId, response)) == NULL) {
        //printf("Message not Found ... Creating one ...\n");
        mex = (message*)malloc(sizeof(message));
        mex->emetteur = (char*)malloc(150 * sizeof(char));
        mex->date = (char*)malloc(150 * sizeof(char));
        addMessage(response, mex);
    } else { //message déjà dans listeMessages
        mex->emetteur = (char*)malloc(150 * sizeof(char));
        mex->date = (char*)malloc(150 * sizeof(char));
        //printf("LE MESSAGE A BIEN ETE' TROUVE' \n");
    }
    mex->entetes = malloc(LINELENGTH * sizeof(char));
    mex->corps = malloc(LINELENGTH * 4 * sizeof(char));

    mex->contentType = sauvegardeEnTetes(mex->entetes, fdesc, mex);

    //printf("En-tetes trouvees:\nFrom: %s\nDate: %s\n", mex->emetteur, mex->date);
    //printf("Content-Type found :%s\n", mex->contentType);
    if (!strncmp(requete, "RETR", strlen("RETR"))) {
        sauvegardeCorps(mex->corps, fdesc);
        //printf("CORPS:\n%sFINCORPS\n", mex->corps);
        //printf("ENTETES:\n%sFIN ENTETES\n", mex->entetes);
        sauvegardeMessage(mex->contentType, mexId, mex->entetes, mex->corps);
    }
}

char* sauvegardeEnTetes(char* entetes, FILE* fdesc, message* mex)
{
    int from = FALSE, date = FALSE, content = FALSE;
    //printf("in analyse entetes ... \n");
    char* contentType = (char*)malloc(150 * sizeof(char));

    char* reponse = entetes;

    //Analyse En-tetes
    while (fgets(reponse, LINELENGTH, fdesc)) {
        //conditions d'arret
        if (!strncmp(".", reponse, strlen("."))) {
            //printf("\nfin entetes ...\n");
            break;
        } //le corps du message va suivre
        if (!strncmp("\n", reponse, strlen("\n"))) {
            //printf("\nfin entetes ...\n");
            break;
        }
        //printf("%s", reponse);

        //sauvegarde en-tetes From et Date
        if (!from) {
            if (!extractEnTete("From: ", reponse, mex->emetteur))
                from = TRUE;
        }
        if (!date) {
            if (!extractEnTete("Date: ", reponse, mex->date))
                date = TRUE;
        }
        if (!content) {
            if (!extractEnTete("Content-Type: ", reponse, contentType))
                content = TRUE;
        }
        reponse = reponse + strlen(reponse);
        //*reponse = '\n';
        //reponse++;
    } //fin while
    if (content) return contentType;
    else return NULL;
}

void sauvegardeCorps(char* reponse, FILE* fdesc)
{
    //sauvegarde du corps
    char* tmp = reponse;

    while (fgets(tmp, LINELENGTH, fdesc)) {
        //printf("tmp:%s", tmp);
        if (!strncmp(".", tmp, strlen("."))) {
            //printf("\nfin CORPS ...\n");
            break;
        }
        tmp = tmp + strlen(tmp);
    } //fin while
}

void sauvegardeMessage(char* contentType, int mexId, char* entetes, char* corps)
{
    char filename[20];

    if (!contentType) { //CONTENT-TYPE ABSENT
        sprintf(filename, "%d.txt", mexId);
        printf("Creation du fichier %s ...\n", filename);
        SauveContenu("txt", mexId, entetes, "w");
        SauveContenu("txt", mexId, corps, "a");
    } else {
        char* result = isMimeSimple(contentType);

        if (!result) {
            sprintf(filename, "%d.txt", mexId);
            printf("Creation du fichier %s ...\n", filename);
            SauveContenu("txt", mexId, entetes, "w");
            SauveContenu("txt", mexId, corps, "a");
        }
        if (!strcmp(result, "multipart")) {
            //printf("WRITING MULTIPART FILE\n");
        } else {
            sprintf(filename, "%d.%s", mexId, result);
            printf("Creation du fichier %s ...\n", filename);
            //printf("WRITING SIMPLE MIME FILE\n");
            SauveContenu(result, mexId, entetes, "w");
            SauveContenu(result, mexId, corps, "a");
        }
    }
}

void SauveContenu(char* extension, int mexId, char* source, char* mode)
{
    char filename[20];
    FILE *fd;

    sprintf(filename, "%d.%s", mexId, extension);
    fd = fopen(filename, mode);

    if (fd == NULL)
        peroraison("fopen", "Impossible d'enregistrer le fichier", 7);
    //printf("Creation du fichier %s\n", filename);

    fwrite(source, sizeof(char), strlen(source), fd);
    fclose(fd);
}

char* isMimeSimple(char* contentType)
{
    char* buf = malloc(1024 * sizeof(char));
    char* fileMimePath = "/etc/mime.types";
    //opening file
    FILE *fd;

    fd = fopen(fileMimePath, "r");
    if (fd == NULL) {
        printf("Impossible d'ouvrir le fichier\n");
        return NULL;
    }
    //reading each line and extracting extension
    while (fgets(buf, 1024, fd)) {
        //printf("LINE:%s", buf);
        if (!strncmp(buf, contentType, strlen(contentType))) {
            return extractExtension(contentType, buf);
        }
    }
    fclose(fd);
    return NULL;
}

char* extractExtension(char* type, char* source)
{
    char* result = malloc(20 * sizeof(char));

    if (!strncmp(type, source, strlen(type))) {
        if (!strncmp("multipart", source, strlen("multipart")))
            return "multipart";
        source += strlen(type);
        //we extract the first extension if present
        char* tmp;
        for (tmp = source; !isalnum(*tmp); tmp++) {
        }
        char* extensionEnd = index(tmp, ' ');
        if (extensionEnd == NULL) extensionEnd = index(tmp, '\n');
        *extensionEnd = 0;
        strcpy(result, tmp);
        return result;
    }
    return NULL;
}
