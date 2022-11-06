/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "https_communication.h"

SSL_CTX *ctx;

int init_ssl_ari(int is_testing) {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    ctx = SSL_CTX_new(TLSv1_2_client_method());
    if(!ctx) {
        return error_msg(SSL_CTX_CONTEXT_FAIL, is_testing);
    }

    return 0;
}
