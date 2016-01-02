#ifndef COMMON_PROTOCOL
#define COMMON_PROTOCOL

#define NB_LIGNE 3
#define NB_COLONNE 3
#define MARGE 3

#define SIZE_MESSAGE_MAX 1500
#define FORBIDEN_CHAR " \n"

//Bot never used
typedef enum Move Move;
enum Move {BOT=2, LEFT=4, RIGHT=6, TOP=8};

#endif
