/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <arpa/inet.h> 
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <time.h>

#define BUFFER_SIZE 2048
#define SOCKET int

void send_request(SOCKET s, char *hostname, char *port, char *path);
SOCKET connect_to_host(char *hostname, char *port);
