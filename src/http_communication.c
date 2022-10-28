/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "http_communication.h"

#define TIMEOUT 5.0

int sock;

void send_request(int sock, char *hostname, char *port, char *path) {
    char buffer[BUFFER_SIZE];

    sprintf(buffer, "GET %s HTTP/1.1\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    //sprintf(buffer + strlen(buffer), "User-Agent: honpwc web_get 1.0\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");

    printf("HTTP request:\n%s\n", buffer);

    send(sock, buffer, strlen(buffer), 0);
}


int connect_to_host(char *hostname, char *port, int is_testing) {
    printf("Configuring remote address...\n");

    struct addrinfo hints;
    struct addrinfo *peer_address;
    struct addrinfo *res;

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
        return 1; //TODO
    }

    if (connect(sock, peer_address->ai_addr, peer_address->ai_addrlen)) {
        freeaddrinfo(peer_address);
        close(sock);
        fprintf(stderr, "connect() failed\n");
        return 1; //TODO
    }

    //printf("Socket varr: %i\n", sock);
    freeaddrinfo(peer_address);
    return 0;
}


int receive_data() {
    char response[BUFFER_SIZE+1];
    char *response_b = response, *queqe;
    char *response_end = response + BUFFER_SIZE;
    char *body = 0;

    enum {length, chunked, connection};
    int encoding = 0;
    int remaining = 0;

    while (1) {
        if (response_b == response_end) {
            fprintf(stderr, "out of buffer space\n");
            return 1;
        }

        int bytes_received = recv(sock, response_b, response_end - response_b, 0);
        if (bytes_received < 1) {
            if (encoding == connection && body) {
                printf("%.*s", (int)(response_end - body), body);
            }

            printf("\nConnection closed by peer.\n");
            break;
        }

        printf("Received (%d bytes): '%.*s'", bytes_received, bytes_received, response_b);
    }

    printf("\nClosing socket...\n");
    close(sock);
    return 0;
}
