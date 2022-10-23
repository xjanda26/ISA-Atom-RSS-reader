/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include <assert.h>
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
#include <err.h>
#include <fcntl.h>
#include <time.h>

void connectRRS();
