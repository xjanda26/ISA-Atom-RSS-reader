/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "error.h"

void print_error_msg (int errorValue, const char* msg) {
    fprintf(stderr,"Error (code: %i). %s", errorValue, msg);
}

int error_msg(const char *msg, int isTesting) {
    int errorValue = ERR_GENERIC;

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

    if (!isTesting) {
        print_error_msg(errorValue, msg);
    }

    return errorValue;
}
