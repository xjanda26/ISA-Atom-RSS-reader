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
#include <time.h>

#include <openssl/ssl.h>

#include "error.h"

#define BUFFER_SIZE 3072
#define TIMEOUT 5.0

extern int sock;

void send_request(int sock, char *hostname, char *port, char *path);
int connect_to_host(char *hostname, char *port, int is_testing);
int receive_data(int is_testing);
