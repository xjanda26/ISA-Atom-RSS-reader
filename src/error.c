/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "error.h"

// Global variables
int exit_value = 0;
int is_testing = 0;

/// @brief Dictionary of error messages
const char *errorMgs[][2] = {
    {OPT_FEW, OPT_FEW_CZ},
    {OPT_UNKNOWN, OPT_UNKNOWN_CZ},
    {OPT_MULTIPLE, OPT_MULTIPLE_CZ},
    {OPT_MUL_COMBINATION, OPT_MUL_COMBINATION_CZ},
    {OPT_MUL_DOMAINS, OPT_MUL_DOMAINS_CZ},
    {OPT_URL_INVALID, OPT_URL_INVALID_CZ},
    {OPT_PATH_MISSING, OPT_PATH_MISSING_CZ},
    {OPT_NO_HOST, OPT_NO_HOST_CZ},
    {OPT_CERT_PATH_MISSING, OPT_CERT_PATH_MISSING_CZ},
    {OPT_FOLDER_PATH_MISSING, OPT_FOLDER_PATH_MISSING_CZ},
    {REGEX_COMPILE_FAILED, REGEX_COMPILE_FAILED_CZ},
    {TCP_UNKNOWN_ADDR, TCP_UNKNOWN_ADDR_CZ},
    {TCP_NO_VALID_DEST, TCP_NO_VALID_DEST_CZ},
    {TCP_CREATE_SOCK_FAIL, TCP_CREATE_SOCK_FAIL_CZ},
    {TCP_CONNECTION_FAIL, TCP_CONNECTION_FAIL_CZ},
    {FILE_READING_FAILED, FILE_READING_FAILED_CZ},
    {FILE_EMPTY_INVALID_URLS, FILE_EMPTY_INVALID_URLS_CZ},
    {SSL_CTX_CONTEXT_FAIL, SSL_CTX_CONTEXT_FAIL_CZ},
    {SSL_OBJECT_FAIL, SSL_OBJECT_FAIL_CZ},
    {SSL_CONNECT_FAIL, SSL_CONNECT_FAIL_CZ},
    {SSL_SERVER_NAME_INDICATION_FAIL, SSL_SERVER_NAME_INDICATION_FAIL_CZ},
    {SSL_GET_DEST_CERTIFICATE_FAIL, SSL_GET_DEST_CERTIFICATE_FAIL_CZ},
    {XML_EMPTY, XML_EMPTY_CZ},
    {XML_PARSING_FAIL, XML_PARSING_FAIL_CZ},
    {XML_CONVERT_FAIL, XML_CONVERT_FAIL_CZ},
    {CERT_LOAD_FILE_FAIL, CERT_LOAD_FILE_FAIL_CZ},
    {CERT_LOAD_FOLDER_FAIL, CERT_LOAD_FOLDER_FAIL_CZ},
    {CERT_DEFAULT_FOLDER_FAIL, CERT_DEFAULT_FOLDER_FAIL_CZ},
    {CERT_VERIFY_FAIL, CERT_VERIFY_FAIL_CZ}
};

/// @brief Collection of error values
const int errorValues[] = {
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
    ERR_TCP_CREATE_SOCK_FAIL,
    ERR_TCP_CONNECTION_FAIL,
    ERR_FILE_READING_FAILED,
    ERR_FILE_EMPTY_INVALID_URLS,
    ERR_SSL_CTX_CONTEXT_FAIL,
    ERR_SSL_OBJECT_FAIL,
    ERR_SSL_CONNECT_FAIL,
    ERR_SSL_SERVER_NAME_INDICATION_FAIL,
    ERR_SSL_GET_DEST_CERTIFICATE_FAIL,
    ERR_XML_EMPTY,
    ERR_XML_PARSING_FAIL,
    ERR_XML_CONVERT_FAIL,
    ERR_CERT_LOAD_FILE_FAIL,
    ERR_CERT_LOAD_FOLDER_FAIL,
    ERR_CERT_DEFAULT_FOLDER_FAIL,
    ERR_CERT_VERIFY_FAIL
};

///@brief Number of simple error cases
int errVariants = 29;

/// @brief Function prints on STDERR error message.
///        Message contains return value and error description. 
/// @param errorValue Return value with wich application exits
/// @param msg Error description
void print_error_msg (int errorValue, const char* msg) {
    if (LANG) {
        fprintf(stderr,"Chyba (kód: %i). %s\n", errorValue, msg);
    } else {
        fprintf(stderr,"Error (code: %i). %s\n", errorValue, msg);
    }
}

/// @brief Function generates return value and error message 
///        based on predefined word.
/// @param msg Predefined error word
/// @return Error value
int error_msg(const char *msg) {
    int errorValue = ERR_GENERIC;
    int msgIndex;

    // Response Error
    if (strstr(msg, HTTP_RESPONSE_BAD_CODE) != NULL) {
        errorValue = ERR_HTTP_RESPONSE_BAD_CODE;
        if (!is_testing) {
            print_error_msg(errorValue, msg);
        }
    } else {
        for (msgIndex = 0; msgIndex < errVariants; msgIndex++) {
            if (!strcmp(msg,errorMgs[msgIndex][0])){
                errorValue = errorValues[msgIndex];
                break;
            }
        }

        if (!is_testing) {
            print_error_msg(errorValue, errorMgs[msgIndex][LANG]);
        }
    }

    exit_value = errorValue;
    return errorValue;
}
