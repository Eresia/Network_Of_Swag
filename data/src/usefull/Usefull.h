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

//Boolean utilisation
typedef enum bool bool;
enum bool{
	false = 0, true = 1
};

//Clean a descriptor after a "fgets"
void clean(const char *, FILE *);

#endif
