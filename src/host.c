/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "host.h"

/*void init_host_array() {
    hosts = (char **) malloc(sizeof(char*));
}*/

int parse_url (char *url, int is_testing) {
    regex_t regex;
    int regexResult;

    regexResult = regcomp(&regex, REGEX_URL, REG_EXTENDED);
    if(regexResult) {
        return error_msg(REGEX_COMPILE_FAILED, is_testing);
    }

    regexResult = regexec(&regex, url, 0, NULL, 0);
    if (regexResult == REG_NOMATCH) {
        return error_msg(OPT_URL_INVALID, is_testing);
    }

    //skús vyparsovať port

    //hosts = url

    regfree(&regex);
    return 0;
}
