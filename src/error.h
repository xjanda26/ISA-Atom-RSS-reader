/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Messages for operator errors
#define OPT_FEW "No argument on STDIN.\n"
#define OPT_UNKNOWN "Unknown operator.\n"
#define OPT_MULTIPLE "Operator is used multpletimes.\n"
#define OPT_MUL_COMBINATION "Multiple combination of operators.\n"
#define OPT_MUL_DOMAINS "Entered multiple domain sources.\n"
#define OPT_URL_INVALID "Entered URL is invalid.\n"
#define OPT_PATH_MISSING "Operator '-f' requires an argument.\n"

#define REGEX_COMPILE_FAILED "Could not compile regex.\n"

// Messages for TCP communication
#define TCP_UNKNOWN_ADDR "DNS doesn't know domain.\n"

// File errors
#define FILE_READING_FAILED "Cannot read file.\n"
#define FILE_EMPTY_INVALID_URLS "File is empty or not containing valid URLs.\n"

// HTTP errors
#define HTTP_RESPONSE_BAD_CODE "Host responsed with stauts code "

//=== ERROR CODES ===
#define ERR_GENERIC 1
#define ERR_REGEX_COMPILE_FAILED 2

#define ERR_OPT_FEW 10
#define ERR_OPT_UNKNOWN 11
#define ERR_OPT_MULTIPLE 12
#define ERR_OPT_MUL_COMBINATION 13
#define ERR_OPT_MUL_DOMAINS 14
#define ERR_OPT_URL_INVALID 15
#define ERR_OPT_PATH_MISSING 16

#define ERR_TCP_UNKNOWN_ADDR 20

#define ERR_FILE_READING_FAILED 30
#define ERR_FILE_EMPTY_INVALID_URLS 31

#define ERR_HTTP_RESPONSE_BAD_CODE 40

void print_error_msg (int errorValue, const char* msg);
int error_msg(const char *msg, int isTesting);
