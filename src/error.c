/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "error.h"

void print_error_msg (int errorValue, const char* msg) {
    fprintf(stderr,"Error (code: %i). %s", errorValue, msg);
}

int error_msg(const char *msg, int isTesting) {
    int errorValue = ERR_GENERIC;

    // OPT Error
    if (!strcmp(msg,OPT_UNKNOWN)){
        errorValue = ERR_OPT_UNKNOWN;
    }

    if (!strcmp(msg, OPT_MULTIPLE)) {
        errorValue = ERR_OPT_MULTIPLE;
    }

    if (!strcmp(msg, OPT_FEW)) {
        errorValue = ERR_OPT_FEW;
    }

    if (!strcmp(msg, OPT_MUL_COMBINATION)) {
        errorValue = ERR_OPT_MUL_COMBINATION;
    }

    if (!strcmp(msg, OPT_MUL_DOMAINS)) {
        errorValue = ERR_OPT_MUL_DOMAINS;
    }

    if (!strcmp(msg, OPT_URL_INVALID)) {
        errorValue = ERR_OPT_URL_INVALID;
    }

    if (!strcmp(msg, OPT_PATH_MISSING)) {
        errorValue = ERR_OPT_PATH_MISSING;
    }

    if (!strcmp(msg, REGEX_COMPILE_FAILED)) {
        errorValue = ERR_REGEX_COMPILE_FAILED;
    }

    if (!strcmp(msg, OPT_NO_HOST)) {
        errorValue = ERR_OPT_NO_HOST;
    }

    // TCP Error
    if (!strcmp(msg, TCP_UNKNOWN_ADDR)) {
        errorValue = ERR_TCP_UNKNOWN_ADDR;
    }

    if (!strcmp(msg, TCP_NO_VALID_DEST)) {
        errorValue = ERR_TCP_NO_VALID_DEST;
    }

    // File Error
    if (!strcmp(msg, FILE_READING_FAILED)) {
        errorValue = ERR_FILE_READING_FAILED;
    }

    if (!strcmp(msg, FILE_EMPTY_INVALID_URLS)) {
        errorValue = ERR_FILE_EMPTY_INVALID_URLS;
    }

    // Response Error
    if (strstr(msg, HTTP_RESPONSE_BAD_CODE) != NULL) {
        errorValue = ERR_HTTP_RESPONSE_BAD_CODE;
    }

    if (!isTesting) {
        print_error_msg(errorValue, msg);
    }

    return errorValue;
}
