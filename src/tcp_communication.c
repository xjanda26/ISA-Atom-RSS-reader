/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "tcp_communication.h"

int sock;
char *xmlResponse;

///TODO: dokumentacia
int connect_to_host(char *hostname, char *port, int is_testing) {
    struct addrinfo hints;
    struct addrinfo *peer_address;
    //struct addrinfo *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;
    
    int getaddRes = getaddrinfo(hostname, port, &hints, &peer_address);
    //printf("DNS res code: %i\n",getaddRes);
    if (getaddRes != 0) {
        fprintf(stderr, "Host name: %s\n", hostname);
        return error_msg(TCP_UNKNOWN_ADDR, is_testing);
    }

    // nepotrebne zatial
    /*for (res = peer_address; res != NULL; res = res->ai_next) {
        printf("DNS cicle\n");
    }*/


    if ((sock = socket(peer_address->ai_family, peer_address->ai_socktype, peer_address->ai_protocol)) == -1) {
        freeaddrinfo(peer_address);
        fprintf(stderr, "Cannot create socket\n");
        ///TODO:
        return 1;
    }

    if (connect(sock, peer_address->ai_addr, peer_address->ai_addrlen)) {
        freeaddrinfo(peer_address);
        close(sock);
        fprintf(stderr, "connect() failed\n");
        ///TODO:
        return 1;
    }

    //printf("Socket varr: %i\n", sock);
    freeaddrinfo(peer_address);
    return 0;
}