/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "http_communication.h"

int sock;

void send_request(int sock, char *hostname, char *port, char *path) {
    char buffer[BUFFER_SIZE];

    sprintf(buffer, "GET %s HTTP/1.1\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    //sprintf(buffer + strlen(buffer), "User-Agent: honpwc web_get 1.0\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");

    //printf("HTTP request:\n%s\n", buffer);

    send(sock, buffer, strlen(buffer), 0);
}


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


int receive_data(int is_testing) {
    char response[BUFFER_SIZE+1];
    char *response_b = response, *tmp_respo_pointer;
    char *response_end = response + BUFFER_SIZE;
    char *body = 0;

    enum {length, chunked, connection};

    int encoding = 0;
    int remaining = 0;
    int header_flag = 0;
    int is_body_without_h = 0;
    //int chunk_flag = 0;
    const clock_t start_time = clock();

    while (1) {
        if ((clock() - start_time) / CLOCKS_PER_SEC > TIMEOUT) {
            fprintf(stderr, "timeout after %.2f seconds\n", TIMEOUT);
            return 1; //TODO
        }

        if (response_b == response_end) {
            fprintf(stderr, "out of buffer space\n");
            return 1; //TODO
        }

        int bytes_received = recv(sock, response_b, response_end - response_b, 0);
        if (bytes_received < 1) {
            printf("\nConnection closed by peer.\n");
            break;
        }

        if(!header_flag) {
            body = strstr(response, "\r\n\r\n");
            if (body) {
                header_flag++;
                *body = 0;
                body += 4; // shift pointer after "\r\n\r\n"
                //printf("Received Headers:\n%s\n====\n\n\n\n", response);

                tmp_respo_pointer = strstr(response, "HTTP/1.1 200 OK");
                if (tmp_respo_pointer) {
                    tmp_respo_pointer = strstr(response, "\nContent-Length: ");
                    if (tmp_respo_pointer) {
                            encoding = length;
                            tmp_respo_pointer = strchr(tmp_respo_pointer, ' ');
                            tmp_respo_pointer += 1;
                            remaining = strtol(tmp_respo_pointer, 0, 10);

                    } else {
                        tmp_respo_pointer = strstr(response, "\nTransfer-Encoding: chunked");
                        if (tmp_respo_pointer) {
                            encoding = chunked;
                            remaining = 0;
                            body += 6; // skip chunk lenght
                        } else {
                            encoding = connection;
                        }
                    }
                    //printf("\nReceived Body:\n");
                } else {
                    tmp_respo_pointer = strstr(response, "HTTP/1.1 ");
                    char http_status_s[4]; 
                    for (int i = 0; i < 3; i++) {
                        http_status_s[i] = tmp_respo_pointer[i+9];
                    }

                    http_status_s[3] = '\0';
                    char *tmp = (char *) malloc((strlen(HTTP_RESPONSE_BAD_CODE)+4) * sizeof(char));
                    strcpy(tmp, HTTP_RESPONSE_BAD_CODE);
                    strcat(tmp, http_status_s);
                    strcat(tmp, "\n");

                    int res = error_msg(tmp, is_testing);
                    free(tmp);
                    close(sock);
                    return res;
                }

                
            }
        }

        if (body && !is_body_without_h) {
            printf("%.*s", bytes_received, body);
            is_body_without_h++;
        } else {
            printf("'%.*s", bytes_received, response);
        }

        //printf("Received (%d bytes): '%.*s'\n\n", bytes_received, bytes_received, response_b);
        //printf("=================================================\n");
    }

    printf("\nClosing socket...\n");
    close(sock);
    return 0;
}
