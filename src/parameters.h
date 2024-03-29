/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include <ctype.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "error.h"
#include "host.h"

// Array indexes
#define A_FLAG 0
#define C_FLAG 1
#define CC_FLAG 2
#define F_FLAG 3
#define T_FLAG 4
#define U_FLAG 5
#define DOMAIN_FLAG 6

extern int optFlags[7];
extern char *filePath;
extern char *certPath;
extern char *certFolder;

void init_parameter_variables();
int parse_hostname(char *argv[], int i);
int parse_parameters (int argc, char *argv[]);
