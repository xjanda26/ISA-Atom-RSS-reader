/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "http_communication.h"

// https://www.ibm.com/docs/en/cics-ts/5.3?topic=client-making-get-requests-atom-feeds-collections

/// @brief Function creates HTTPv1.0 request and sends it by socket.
/// @param sock Socket
/// @param hostname Name of remote host
/// @param port Destination port
/// @param path Path to the resource on remote host
void send_http_request(int sock, char *hostname, char *port, char *path) {
    char buffer[BUFFER_SIZE];

    sprintf(buffer, "GET %s HTTP/1.0\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");

    send(sock, buffer, strlen(buffer), 0);
}

/// @brief Function saves received data on socket into dynamically enlarged array.
///        Function saves data only if HTTP status is 200, otherwise it returns an error.
///        An error might occur even after timeout of 5 seconds.
/// @return Zero value on success or TCP or HTTP error type defined in error.h
int receive_data() {
    char response[BUFFER_SIZE+1];
    char *response_b = response, *tmp_respo_pointer;
    char *response_end = response + BUFFER_SIZE;
    char *body = 0;

    int header_flag = 0;
    int is_body_without_h = 0;

    const clock_t start_time = clock();

    memset(response, '\0', sizeof(response));
    while (1) {
        if ((clock() - start_time) / CLOCKS_PER_SEC > TIMEOUT) {
            fprintf(stderr, "timeout after %.2f seconds.\n", TIMEOUT);
            close(sock);
            return ERR_TCP_TIMEOUT;
        }

        int bytes_received = recv(sock, response_b, response_end - response_b, 0);
        response[BUFFER_SIZE+1] = '\0';

        // Connection closed by peer
        if (bytes_received < 1) {
            break;
        }

        if(!header_flag) {
            body = strstr(response, "\r\n\r\n");
            if (body) {
                header_flag++;
                *body = 0;
                body += 4; // shift pointer after "\r\n\r\n"

                // https://www.ibm.com/docs/en/cics-ts/5.2?topic=concepts-status-codes-reason-phrases#dfhtl_httpstatus
                ///TODO: otetsovat aj pre http 1.0
                tmp_respo_pointer = strstr(response, "200 OK");
                if (!tmp_respo_pointer) {
                    ///TODO: otestovat pre http 1.0/1.1
                    tmp_respo_pointer = strstr(response, "HTTP/1.");
                    char http_status_s[4];

                    // Extracting HTTP STATUS and creating error msg
                    for (int i = 0; i < 3; i++) {
                        http_status_s[i] = tmp_respo_pointer[i+9];
                    }

                    http_status_s[3] = '\0';
                    char *tmp = (char *) malloc((strlen(HTTP_RESPONSE_BAD_CODE)+4) * sizeof(char));
                    strcpy(tmp, HTTP_RESPONSE_BAD_CODE);
                    strcat(tmp, http_status_s);
                    strcat(tmp, "\n");

                    error_msg(tmp);
                    free(tmp);
                    close(sock);
                    return exit_value;
                }
            }
        }

        if (body && !is_body_without_h) {
            // Part of data right after HTTP header
            xmlResponse = (char*) malloc((strlen(body) + 1) * sizeof(char));
            strcpy(xmlResponse, body);
            strcat(xmlResponse, "\0");
            is_body_without_h = 1;        
        } else {
            size_t respo_len = strlen(response);
            tmp_respo_pointer = strstr(response, "\r\n0");

            if (tmp_respo_pointer) {
                // The end of received data
                size_t respo_len_until_end = respo_len - strlen(tmp_respo_pointer);
                xmlResponse = (char*) realloc(xmlResponse, (strlen(xmlResponse) + respo_len_until_end + 1) * sizeof(char));
                strncat(xmlResponse, response, respo_len_until_end);
            } else {
                // The middle of received data
                xmlResponse = (char*) realloc(xmlResponse, (strlen(response) + strlen(xmlResponse) + 1) * sizeof(char));
                strcat(xmlResponse, response);
            }
        }

        // Clearing an array for new incoming data
        memset(response, '\0', sizeof(response));
    }

    close(sock);
    return SUCCESS;
}
