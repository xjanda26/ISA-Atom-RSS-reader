/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "error.h"

char *errorMgs[] = {
    OPT_FEW,
    OPT_UNKNOWN,
    OPT_MULTIPLE,
    OPT_MUL_COMBINATION,
    OPT_MUL_DOMAINS,
    OPT_URL_INVALID,
    OPT_PATH_MISSING,
    OPT_NO_HOST,
    OPT_CERT_PATH_MISSING,
    OPT_FOLDER_PATH_MISSING,
    REGEX_COMPILE_FAILED,
    TCP_UNKNOWN_ADDR,
    TCP_NO_VALID_DEST,
    FILE_READING_FAILED,
    FILE_EMPTY_INVALID_URLS,
    SSL_CTX_CONTEXT_FAIL,
    SSL_OBJECT_FAIL,
    SSL_CONNECT_FAIL,
    SSL_SERVER_NAME_INDICATION_FAIL,
    SSL_GET_DEST_CERTIFICATE_FAIL
};

int errorValues[] = {
    ERR_OPT_FEW,
    ERR_OPT_UNKNOWN,
    ERR_OPT_MULTIPLE,
    ERR_OPT_MUL_COMBINATION,
    ERR_OPT_MUL_DOMAINS,
    ERR_OPT_URL_INVALID,
    ERR_OPT_PATH_MISSING,
    ERR_OPT_NO_HOST,
    ERR_OPT_CERT_PATH_MISSING,
    ERR_OPT_FOLDER_PATH_MISSING,
    ERR_REGEX_COMPILE_FAILED,
    ERR_TCP_UNKNOWN_ADDR,
    ERR_TCP_NO_VALID_DEST,
    ERR_FILE_READING_FAILED,
    ERR_FILE_EMPTY_INVALID_URLS,
    ERR_SSL_CTX_CONTEXT_FAIL,
    ERR_SSL_OBJECT_FAIL,
    ERR_SSL_CONNECT_FAIL,
    ERR_SSL_SERVER_NAME_INDICATION_FAIL,
    ERR_SSL_GET_DEST_CERTIFICATE_FAIL
};

int errVariants = 20;

void print_error_msg (int errorValue, const char* msg) {
    fprintf(stderr,"Error (code: %i). %s", errorValue, msg);
}

int error_msg(const char *msg, int isTesting) {
    int errorValue = ERR_GENERIC;

    // Response Error
    if (strstr(msg, HTTP_RESPONSE_BAD_CODE) != NULL) {
        errorValue = ERR_HTTP_RESPONSE_BAD_CODE;
    } else {
        for (int i = 0; i < errVariants; i++) {
            if (!strcmp(msg,errorMgs[i])){
                errorValue = errorValues[i];
                break;
            }
        }
    }

    if (!isTesting) {
        print_error_msg(errorValue, msg);
    }

    return errorValue;
}
