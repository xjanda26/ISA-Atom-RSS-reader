/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "https_communication.h"

SSL *ssl;
SSL_CTX *ctx;

/// @brief Function initializes SSL libraries and loads one or more
///        certificates depends on input from user.
/// @return Zero value on success or SSL or CERT error type defined in error.h
int init_ssl() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    ctx = SSL_CTX_new(TLSv1_2_client_method());
    if(!ctx) {
        return error_msg(SSL_CTX_CONTEXT_FAIL);
    }

    if (optFlags[C_FLAG] > 0) {
        if (certPath) {
            if (SSL_CTX_load_verify_file(ctx, certPath) != 1) {
                error_msg(CERT_LOAD_FILE_FAIL);
                return exit_value;
            }
        }
    } else if (optFlags[CC_FLAG] > 0) {
        if (certFolder) {
            if (SSL_CTX_load_verify_dir(ctx, certFolder) != 1) {
                error_msg(CERT_LOAD_FOLDER_FAIL);
                return exit_value;
            }
        }
    } else if (SSL_CTX_set_default_verify_paths(ctx) != 1) {
        SSL_CTX_free(ctx);
        error_msg(CERT_DEFAULT_FOLDER_FAIL);
        return exit_value;
    }

    return SUCCESS;
}

/// @brief Function initializes SSL communication and starts it
/// @param hostname Name of remote host
/// @return Zero value on success or SSL error type defined in error.h
int init_tls_connection(char *hostname) {
    ssl = SSL_new(ctx);

    if (!ssl) {
        return error_msg(SSL_OBJECT_FAIL);
    }

    if (!SSL_set_tlsext_host_name(ssl, hostname)) {
        return error_msg(SSL_SERVER_NAME_INDICATION_FAIL);
    }

    SSL_set_fd(ssl, sock);
    if (SSL_connect(ssl) == -1) {
        return error_msg(SSL_CONNECT_FAIL);
    }

    return SUCCESS;
}

/// @brief Function gets host's certificate and verify it with loaded certificates
/// @return Zero value on success or SSL or CERT error type defined in error.h
int verify_certificate() {
    X509 *cert = SSL_get_peer_certificate(ssl);
    if (!cert) {
        return error_msg(SSL_GET_DEST_CERTIFICATE_FAIL);
    }

    X509_free(cert);

    int verifyResult = SSL_get_verify_result(ssl);
    if (verifyResult != X509_V_OK) {
        error_msg(CERT_VERIFY_FAIL);
        return exit_value;
    }

    return SUCCESS;
}

/// @brief Function creates HTTPv1.0 request and sends it by socket.
/// @param sock Socket
/// @param hostname Name of remote host
/// @param port Destination port
/// @param path Path to the resource on remote host
void send_https_request(char *hostname, char *port, char *path) {
    char buffer[BUFFER_SIZE];

    sprintf(buffer, "GET %s HTTP/1.0\r\n", path);
    sprintf(buffer + strlen(buffer), "Host: %s:%s\r\n", hostname, port);
    sprintf(buffer + strlen(buffer), "Connection: close\r\n");
    sprintf(buffer + strlen(buffer), "\r\n");

    SSL_write(ssl, buffer, strlen(buffer));
}

/// @brief Function saves received data by SSL function on socket
///        into dynamically enlarged array. Function saves data only
///        if HTTP status is 200, otherwise it returns an error.
///        An error might occur even after timeout of 5 seconds.
/// @return Zero value on success or TCP or HTTP error type defined in error.h
int receive_ssl_data() {
    char response[BUFFER_SIZE+1];
    char *tmp_respo_pointer;
    char *body = 0;

    int header_flag = 0;
    int is_body_without_h = 0;

    const clock_t start_time = clock();

    memset(response, '\0', sizeof(response));
    while (1) {
        if ((clock() - start_time) / CLOCKS_PER_SEC > TIMEOUT) {
            fprintf(stderr, "timeout after %.2f seconds.\n", TIMEOUT);
            return ERR_TCP_TIMEOUT;
        }

        int bytes_received = SSL_read(ssl, response, sizeof(response));
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
                tmp_respo_pointer = strstr(response, "200 OK");
                if (!tmp_respo_pointer) {
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
            memcpy(xmlResponse, body, strlen(body) + 1);
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
                xmlResponse = (char*) realloc(xmlResponse, (strlen(response) + strlen(xmlResponse) + 2) * sizeof(char));
                strcat(xmlResponse, response);
            }
        }

        // Clearing an array for new incoming data
        memset(response, '\0', sizeof(response));
    }

    SSL_shutdown(ssl);
    close(sock);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    return SUCCESS;
}
