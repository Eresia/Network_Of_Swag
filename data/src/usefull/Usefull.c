#include "Usefull.h"

char* getString(){
    char* string = calloc(100, sizeof(char));
    fgets(string, 100, stdin);
    clean(string, stdin);
    return string;
}

void clean(const char *buffer, FILE *fp)
{
    char *p = strchr(buffer,'\n');
    if (p != NULL)
    *p = 0;
    else
    {
        int c;
        while ((c = fgetc(fp)) != '\n' && c != EOF);
    }
}
