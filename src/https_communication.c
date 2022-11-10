/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "https_communication.h"

SSL *ssl;
SSL_CTX *ctx;


int init_ssl(int is_testing) {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    ctx = SSL_CTX_new(TLSv1_2_client_method());
    if(!ctx) {
        return error_msg(SSL_CTX_CONTEXT_FAIL, is_testing);
    }

    if (optFlags[C_FLAG] > 0) {
        if (certPath) {
            if (SSL_CTX_load_verify_file(ctx, certPath) != 1) {
                printf("Failed to load cert file\n");
                return 1;
            }
        }
    } else if (optFlags[CC_FLAG] > 0) {
        if (certFolder) {
            if (SSL_CTX_load_verify_dir(ctx, certFolder) != 1) {
                printf("Failed to load cert folder\n");
                return 1;
            }
        }
    } else if (SSL_CTX_set_default_verify_paths(ctx) != 1) {
        SSL_CTX_free(ctx);
        printf("Failed while setting default path\n");
        return 1;
    }

    return 0;
}

int init_tls_connection(char *hostname, int is_testing) {
    ssl = SSL_new(ctx);

    if (!ssl) {
        return error_msg(SSL_OBJECT_FAIL, is_testing);
    }

    if (!SSL_set_tlsext_host_name(ssl, hostname)) {
        return error_msg(SSL_SERVER_NAME_INDICATION_FAIL, is_testing);
    }

    SSL_set_fd(ssl, sock);
    if (SSL_connect(ssl) == -1) {
        return error_msg(SSL_CONNECT_FAIL, is_testing);
    }

    printf ("SSL/TLS using %s\n", SSL_get_cipher(ssl));
    return 0;
}

int verify_certificate(int is_testing) {
    X509 *cert = SSL_get_peer_certificate(ssl);
    if (!cert) {
        return error_msg(SSL_GET_DEST_CERTIFICATE_FAIL, is_testing);
    }

    /*char *tmp;
    if ((tmp = X509_NAME_oneline(X509_get_subject_name(cert),0,0))) {
        printf("subject: %s\n", tmp);
        OPENSSL_free(tmp);
    }

    if ((tmp = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0))) {
        printf("issuer: %s\n", tmp);
        OPENSSL_free(tmp);
    }*/


    X509_free(cert);

    int verifyResult = SSL_get_verify_result(ssl);
    if (verifyResult != X509_V_OK) {
        printf("Error while verifying  cert\n");
        return 1;
    }

    return 0;
}

void send_https_request(char *hostname, char *port, char *path) {
    char buffer[BUFFER_SIZE];

    sprintf(buffer, "GET %s HTTP/1.0\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");

    SSL_write(ssl, buffer, strlen(buffer));
}

int receive_ssl_data(int is_testing) {
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

        int bytes_received = SSL_read(ssl, response, sizeof(response));
        //printf("%s\n\n", response);
        response[BUFFER_SIZE+1] = '\0';
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
                //*body = 0;
                body += 4; // shift pointer after "\r\n\r\n"
                //printf("Received Headers:\n%s\n====\n\n\n\n", response);
                // https://www.ibm.com/docs/en/cics-ts/5.2?topic=concepts-status-codes-reason-phrases#dfhtl_httpstatus
                tmp_respo_pointer = strstr(response, "HTTP/1.1 200 OK");
                if (tmp_respo_pointer) {
                    /*tmp_respo_pointer = strstr(response, "\nContent-Length: ");
                    if (tmp_respo_pointer) {
                            //encoding = length;
                            tmp_respo_pointer = strchr(tmp_respo_pointer, ' ');
                            tmp_respo_pointer += 1;
                            //remaining = strtol(tmp_respo_pointer, 0, 10);

                    } else {
                        tmp_respo_pointer = strstr(response, "\nTransfer-Encoding: chunked");
                        if (tmp_respo_pointer) {
                            //encoding = chunked;
                            //body += 6; // skip chunk lenght
                        } else {
                            //encoding = connection;
                        }
                    }*/
                    //printf("\nReceived Body2:\n\n%s\n\n", body);
                } else {
                    tmp_respo_pointer = strstr(response, "HTTP/1.");
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
            //printf("\nBody-h length: %i\n", strlen(body));
            //printf("Body-h Bytes: %i\n", bytes_received);
            //printf("BODY\n\n\n");
            xmlResponse = (char*) malloc((strlen(body) + 1) * sizeof(char));
            //strcpy(xmlResponse, body);
            memcpy(xmlResponse, body, strlen(body) + 1);
            //strcat(xmlResponse, "\0");
            //printf("\n==Body L:%i\n%s\n==\nStrlen copied XML: %i\n\n",sizeof(body), body, sizeof(xmlResponse));
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
                xmlResponse = (char*) realloc(xmlResponse, (strlen(response) + strlen(xmlResponse) + 2) * sizeof(char));
                //printf("Builded XML size after: %i\n", strlen(xmlResponse));
                strcat(xmlResponse, response);
                
                /*if (is_body_without_h == 1) {
                    printf("\n==Response L:%i\n%s\n==\n",strlen(response), response);
                    is_body_without_h++;
                }*/

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
    printf("%s", xmlResponse);

    //printf("\nClosing socket...\n");
    SSL_shutdown(ssl);
    close(sock);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    return 0;
}
