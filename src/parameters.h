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

int parse_parameters (int argc, char *argv[], int is_testing);
