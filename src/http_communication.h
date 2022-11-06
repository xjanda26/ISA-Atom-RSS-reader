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

#include "error.h"
#include "tcp_communication.h"

#define BUFFER_SIZE 1536
#define TIMEOUT 5.0

void send_http_request(int sock, char *hostname, char *port, char *path);
int receive_data(int is_testing);
