/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "error.h"
#include "parameters.h"
#include "tcp_communication.h"

#define BUFFER_SIZE 2048
#define TIMEOUT 5.0

extern SSL_CTX *ctx;

int init_ssl();
int init_tls_connection(char *hostname);
int verify_certificate();
void send_https_request(char *hostname, char *port, char *path);
int receive_ssl_data();
