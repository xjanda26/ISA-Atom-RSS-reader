/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OPT_FEW "No argument on STDIN.\n"
#define OPT_UNKNOWN "Unknown operator.\n"
#define OPT_MULTIPLE "Operator is used multpletimes.\n"
#define OPT_MUL_COMBINATION "Multiple combination of operators.\n"
#define OPT_MUL_DOMAINS "Entered multiple domain sources.\n"

#define ERR_GENERIC 1

#define ERR_OPT_FEW 10
#define ERR_OPT_UNKNOWN 11
#define ERR_OPT_MULTIPLE 12
#define ERR_OPT_MUL_COMBINATION 13
#define ERR_OPT_MUL_DOMAINS 14

void print_error_msg (int errorValue, const char* msg);
int error_msg(const char *msg, int isTesting);
