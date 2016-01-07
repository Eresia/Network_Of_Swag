#ifndef USEFULL
#define USEFULL

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Returns and errors :
0 : No error
1 : Bad number of arguments
2 : Incorrect arguments
3 : Other

*/
#define NO_ERROR 0
#define BAD_NUMBER_OF_ARGUMENTS 1
#define INCORRECT_ARGUMENT 2
#define OTHER_ERROR 3

#define HEIGHT 700 /*Taille de la fenêtre*/
#define WIDTH 1000

#define PIX_PER_BLOCK 24

#define NB_LIGNE ((WIDTH/PIX_PER_BLOCK) +1)
#define NB_COLONNE ((HEIGHT/PIX_PER_BLOCK) + 1)

/*Boolean utilisation*/
enum bool{
	false = 0, true = 1
};
typedef enum bool bool;

/*Clean a descriptor after a "fgets"*/
void clean(const char *, FILE *);

bool startsWith(const char *, const char *);
char* take_begin(char*, int, char*, int);

#endif
