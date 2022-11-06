/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "error.h"

extern SSL_CTX *ctx;

int init_ssl_ari(int is_testing);
