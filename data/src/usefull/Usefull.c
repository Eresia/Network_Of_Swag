#include <string.h>
#include "Usefull.h"

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

/* Dit si str commence par pre*/
bool startsWith(const char *str, const char *pre) {
    size_t lenpre = strlen(pre),
           lenstr = strlen(str);
    return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

char* take_begin(char* str, int sizeMax, char* caracs, int nbCaracs){
	int i, j;
	char* result = calloc(sizeMax, sizeof(char));
	for(i = 0; i < sizeMax; i++){
		for(j = 0; j < nbCaracs; j++){
			if(str[i] == caracs[j]){
				return result;
			}
		}
		result[i] = str[i];
	}
	return result;
}
