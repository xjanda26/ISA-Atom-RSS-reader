/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "http_communication.h"

char *xmlResponse;

// https://www.ibm.com/docs/en/cics-ts/5.3?topic=client-making-get-requests-atom-feeds-collections
void send_request(int sock, char *hostname, char *port, char *path) {
    char buffer[BUFFER_SIZE];

    sprintf(buffer, "GET %s HTTP/1.0\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    //sprintf(buffer + strlen(buffer), "User-Agent: honpwc web_get 1.0\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");

    //printf("HTTP request:\n%s\n", buffer);

    send(sock, buffer, strlen(buffer), 0);
}

int receive_data(int is_testing) {
    char response[BUFFER_SIZE+1];
    char *response_b = response, *tmp_respo_pointer;
    char *response_end = response + BUFFER_SIZE;
    char *body = 0;

    //enum {length, chunked, connection};

    //int encoding = 0;
    //int remaining = 0;
    int header_flag = 0;
    int is_body_without_h = 0;
    //int chunk_flag = 0;
    const clock_t start_time = clock();

    int debug = 0;
    memset(response, '\0', sizeof(response));
    while (1) {
        //printf("DEBUG counter: %i\n", debug);
        //if (debug == 4)
        //    break;
        if ((clock() - start_time) / CLOCKS_PER_SEC > TIMEOUT) {
            fprintf(stderr, "timeout after %.2f seconds\n", TIMEOUT);
            return 1; //TODO
        }

        if (response_b == response_end) {
            fprintf(stderr, "out of buffer space\n");
            return 1; //TODO
        }

        int bytes_received = recv(sock, response_b, response_end - response_b, 0);
        //printf("%s\n\n", response);
        response[BUFFER_SIZE] = '\0';
        //printf("\nB: %i, HF:%i\n", bytes_received, header_flag);
        if (bytes_received < 1) {
            //printf("\nConnection closed by peer.\n");
            break;
        }

        if(!header_flag) {
            body = strstr(response, "\r\n\r\n");
            //printf("%s\n\n", response);
            if (body) {
                header_flag++;
                *body = 0;
                body += 4; // shift pointer after "\r\n\r\n"
                //printf("Received Headers:\n%s\n====\n\n\n\n", response);

                // https://www.ibm.com/docs/en/cics-ts/5.2?topic=concepts-status-codes-reason-phrases#dfhtl_httpstatus
                tmp_respo_pointer = strstr(response, "HTTP/1.1 200 OK");
                if (tmp_respo_pointer) {
                    tmp_respo_pointer = strstr(response, "\nContent-Length: ");
                    if (tmp_respo_pointer) {
                            //encoding = length;
                            tmp_respo_pointer = strchr(tmp_respo_pointer, ' ');
                            tmp_respo_pointer += 1;
                            //remaining = strtol(tmp_respo_pointer, 0, 10);

                    } else {
                        tmp_respo_pointer = strstr(response, "\nTransfer-Encoding: chunked");
                        if (tmp_respo_pointer) {
                            //encoding = chunked;
                            body += 6; // skip chunk lenght
                        } else {
                            //encoding = connection;
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
            //printf("%.*s", bytes_received, body);
            //printf("Body-h length: %i\n", strlen(body));
            //printf("Body-h Bytes: %i\n", bytes_received);
            //printf("BODY\n\n\n");
            xmlResponse = (char*) malloc((strlen(body) + 1) * sizeof(char));
            strcpy(xmlResponse, body);
            strcat(xmlResponse, "\0");
            //printf("\n==Body L:%i\n%s\n==\n",strlen(body), body);
            is_body_without_h = 1;
            if (debug > 0) {
                printf("%s", xmlResponse);
                printf("\n\n\n WTF... %i\n", debug);
            }
        
        } else {
            size_t respo_len = strlen(response);
            tmp_respo_pointer = strstr(response, "\r\n0");

            if (tmp_respo_pointer) {
                //printf("TMP poiner is ==NOT== null\n");
                //printf("Debug counter stops on: %i\n", debug);
                //printf("%s\n", tmp_respo_pointer);
                //break;
                size_t respo_len_until_end = respo_len - strlen(tmp_respo_pointer);
                xmlResponse = (char*) realloc(xmlResponse, (strlen(xmlResponse) + respo_len_until_end + 1) * sizeof(char));
                strncat(xmlResponse, response, respo_len_until_end);
                //printf("\n==\n%s\n==\n", response);
            } else {
                //printf("TMP poiner is null\n");
                //printf("%s\n\n", response);
                //printf("Builded XML size before: %i\n", strlen(xmlResponse));
                xmlResponse = (char*) realloc(xmlResponse, (strlen(response) + strlen(xmlResponse) + 1) * sizeof(char));
                //printf("Builded XML size after: %i\n", strlen(xmlResponse));
                strcat(xmlResponse, response);
                
                //printf("\n==Response L:%i\n%s\n==\n",strlen(response), response);

            }
            //printf("%s\n\n\n\n", xmlResponse);
            //printf("XML length: %i\n", (strlen(response) + strlen(xmlResponse)));
            //printf("Body Bytes: %i\n", bytes_received);
        }

        memset(response, '\0', sizeof(response));
        //printf("\n==XML builded\n%s\n\n", xmlResponse);
        debug++;

        //printf("%s", xmlResponse);
        //strcpy(response,"");

        //printf("Received (%d bytes): '%.*s'\n\n", bytes_received, bytes_received, response_b);
        //printf("=================================================\n");
    }

    //printf("\n\n\n==FINAL L:%i\n%s", strlen(xmlResponse), xmlResponse);
    //printf("%s", xmlResponse);

    //printf("\nClosing socket...\n");
    close(sock);
    return 0;
}
