/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// EN/CZ language switch, CZ == 1, EN == 0
#define LANG 1

//=== Dictionary of error messages ===
// Messages for operator errors
// EN
#define OPT_FEW "No argument on STDIN."
#define OPT_UNKNOWN "Unknown operator."
#define OPT_MULTIPLE "Operator is used multpletimes."
#define OPT_MUL_COMBINATION "Multiple combination of operators."
#define OPT_MUL_DOMAINS "Entered multiple domain sources."
#define OPT_URL_INVALID "Entered URL is invalid."
#define OPT_PATH_MISSING "Operator '-f' requires an argument with file name."
#define OPT_NO_HOST "No hostname nor file with hostnames entered."
#define OPT_CERT_PATH_MISSING "Operator '-c' requires an argument with file name."
#define OPT_FOLDER_PATH_MISSING "Operator '-C' requires an argument with folder name."

#define REGEX_COMPILE_FAILED "Could not compile regex."
// CZ
#define OPT_FEW_CZ "Žádný argument na standardním vstupu."
#define OPT_UNKNOWN_CZ "Neznámý vstupní argument."
#define OPT_MULTIPLE_CZ "Vstupní argument byl použit vícekrát."
#define OPT_MUL_COMBINATION_CZ "Vícenásobná kombinace vstupních argumentů stejného typu."
#define OPT_MUL_DOMAINS_CZ "Vloženo více zdrojů cílových destinací."
#define OPT_URL_INVALID_CZ "Vložena URL není ve validním tvaru."
#define OPT_PATH_MISSING_CZ "Vstupní argument '-f' vyžaduje soubor."
#define OPT_NO_HOST_CZ "Žádny zdroj cílové destinace nebyl vložen."
#define OPT_CERT_PATH_MISSING_CZ "Vstupní argument '-c' vyžaduje soubor."
#define OPT_FOLDER_PATH_MISSING_CZ "Vstupní argument '-C' vyžaduje složku."

#define REGEX_COMPILE_FAILED_CZ "Nepodařilo se přeložit regulární výraz."

// Messages for TCP communication
// EN
#define TCP_UNKNOWN_ADDR "DNS doesn't know domain."
#define TCP_NO_VALID_DEST "Any valid destination."
#define TCP_CREATE_SOCK_FAIL "Failed to create socked."
#define TCP_CONNECTION_FAIL "Failed while creating connection to host."

// CZ
#define TCP_UNKNOWN_ADDR_CZ "DNS rezoluce nezná cílovou destinaci."
#define TCP_NO_VALID_DEST_CZ "Žádná validní cílová destinace nebyla vložena."
#define TCP_CREATE_SOCK_FAIL_CZ "Selhání při tvoření schránky."
#define TCP_CONNECTION_FAIL_CZ "Selhání při vytváření spojení s cílovou destinací."

// File errors
// EN
#define FILE_READING_FAILED "Cannot read file."
#define FILE_EMPTY_INVALID_URLS "File is empty or not containing valid URLs."

//CZ
#define FILE_READING_FAILED_CZ "Nepodařilo se přečíst vložený soubor."
#define FILE_EMPTY_INVALID_URLS_CZ "Soubor je prázdný nebo neobsahuje validní URL."

// HTTP errors
// EN
#define HTTP_RESPONSE_BAD_CODE "Host responsed with status code "

// CZ
#define HTTP_RESPONSE_BAD_CODE_CZ "Cílová destinace odpověděla s kódem "

// SSL errors
// EN
#define SSL_CTX_CONTEXT_FAIL "Fail while creating CTX context."
#define SSL_OBJECT_FAIL "Fail while creating SSL object."
#define SSL_CONNECT_FAIL "Cannot SSL connect to destination."
#define SSL_SERVER_NAME_INDICATION_FAIL "Server Name Indication failed."
#define SSL_GET_DEST_CERTIFICATE_FAIL "Getting destination certificate failed."

// CZ
#define SSL_CTX_CONTEXT_FAIL_CZ "Selhání při tvoření CTX kontextu."
#define SSL_OBJECT_FAIL_CZ "Selhání při tvoření SSL objektu."
#define SSL_CONNECT_FAIL_CZ "Nezdařilo se SSL připojení k cílové destinaci."
#define SSL_SERVER_NAME_INDICATION_FAIL_CZ "Selhala identifikace jména serveru."
#define SSL_GET_DEST_CERTIFICATE_FAIL_CZ "Selhalo získaní certifikátu cílové destinace."

// XML errors
// EN
#define XML_EMPTY "XML file is empty."
#define XML_PARSING_FAIL "Parsing XML file failed."
#define XML_CONVERT_FAIL "Converting to XML string failed."

// CZ
#define XML_EMPTY_CZ "XML soubor je prázdný."
#define XML_PARSING_FAIL_CZ "Selhalo zpracování XML souboru."
#define XML_CONVERT_FAIL_CZ "Selhalo konvertování XML řetězce."

// Certificate errors
// EN
#define CERT_LOAD_FILE_FAIL "Fail while loading certificate file."
#define CERT_LOAD_FOLDER_FAIL "Fail while loading certificates folder."
#define CERT_DEFAULT_FOLDER_FAIL "Fail while setting default certificates folder."
#define CERT_VERIFY_FAIL "Failed to verify certificate."

// CZ
#define CERT_LOAD_FILE_FAIL_CZ "Selhání při čtení certifikačního souboru."
#define CERT_LOAD_FOLDER_FAIL_CZ "Selhání při čtení certifikační složky."
#define CERT_DEFAULT_FOLDER_FAIL_CZ "Selhání při nastavování výchozí certifikační složky."
#define CERT_VERIFY_FAIL_CZ "Selhalo ověření certifikátu cílové destinace."

//=== Collection of error values ===
#define SUCCESS 0
#define ERR_GENERIC 1
#define ERR_REGEX_COMPILE_FAILED 2

#define ERR_OPT_FEW 10
#define ERR_OPT_UNKNOWN 11
#define ERR_OPT_MULTIPLE 12
#define ERR_OPT_MUL_COMBINATION 13
#define ERR_OPT_MUL_DOMAINS 14
#define ERR_OPT_URL_INVALID 15
#define ERR_OPT_PATH_MISSING 16
#define ERR_OPT_NO_HOST 17
#define ERR_OPT_CERT_PATH_MISSING 18
#define ERR_OPT_FOLDER_PATH_MISSING 19

#define ERR_TCP_UNKNOWN_ADDR 20
#define ERR_TCP_NO_VALID_DEST 21

#define ERR_TCP_TIMEOUT 22

#define ERR_TCP_CREATE_SOCK_FAIL 23
#define ERR_TCP_CONNECTION_FAIL 24

#define ERR_FILE_READING_FAILED 30
#define ERR_FILE_EMPTY_INVALID_URLS 31

#define ERR_HTTP_RESPONSE_BAD_CODE 40

#define ERR_SSL_CTX_CONTEXT_FAIL 50
#define ERR_SSL_OBJECT_FAIL 51
#define ERR_SSL_CONNECT_FAIL 52
#define ERR_SSL_SERVER_NAME_INDICATION_FAIL 53
#define ERR_SSL_GET_DEST_CERTIFICATE_FAIL 54

#define ERR_XML_EMPTY 60
#define ERR_XML_PARSING_FAIL 61
#define ERR_XML_CONVERT_FAIL 62

#define ERR_CERT_LOAD_FILE_FAIL 70
#define ERR_CERT_LOAD_FOLDER_FAIL 71
#define ERR_CERT_DEFAULT_FOLDER_FAIL 72
#define ERR_CERT_VERIFY_FAIL 73

extern int exit_value;
extern int is_testing;

void print_error_msg (int errorValue, const char* msg);
int error_msg(const char *msg);
