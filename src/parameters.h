/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#define _POSIX_C_SOURCE 2

#include <ctype.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "error.h"

// Array indexes
#define A_FLAG 0
#define C_FLAG 1
#define CC_FLAG 2
#define F_FLAG 3
#define T_FLAG 4
#define U_FLAG 5
#define DOMAIN_FLAG 6 

#define REGEX_URL "^https?:\\/\\/www\\.[a-zA-Z0-9]+([-\\.]{1}[a-zA-Z0-9]+)*\\.[a-z]{2,5}(:([1-9][0-9]{0,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5]))?(\\/.*)?$"

extern int optFlags[7];
extern char *hostName;
extern char *port;
extern char *filePath;

void init_parameter_variables();
int parse_parameters (int argc, char *argv[], int is_testing);
