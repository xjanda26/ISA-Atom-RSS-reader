/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "connectionAri.h"

void connectRRS() {
  printf("Connecting\n");

  /*
  int sock;

  if((sock = socket(AF_INET, SOCK_STREAM,0)) == -1) {
    fprintf(stderr, "Unable to create socket\n");
    exit(1);
  }
  
  char response[8192];
  char *p = response, *q;
  char *end = response + 8191;
  char *body = 0;

  enum {
    length,
    chunked,
    connection
  };

  int encoding = 0;
  int remaining = 0;

  //receiving data
  while (1) {
    if (p == end) {
      fprintf(stderr, "out of buffer space \n");
      close(sock);
      exit(1);
    }

    fd_set reads;
    FD_ZERO(&reads);
    //FD_SET(servent, &reads);
  }
  */
}
