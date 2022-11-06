/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "error.h"

extern int sock;

int connect_to_host(char *hostname, char *port, int is_testing);
