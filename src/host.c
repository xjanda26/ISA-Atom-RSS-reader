/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "host.h"

char **hosts;
char **paths;
int *ports;

int destinations_counter = 0;


void init_destinations() {
    hosts = malloc(sizeof(char*));
    paths = malloc(sizeof(char*));
    ports = (int*) malloc(sizeof(int));
}

void clear_destinations() {
    for (int i = 0; i < destinations_counter; i++) {
        free(hosts[i]);
        free(paths[i]);
    }

    free(hosts);
    free(paths);
    free(ports);
}

int parse_url (char *url, int is_testing) {
    regex_t regex;

    int port = 80;
    char *hostname;
    char *path;

    if(regcomp(&regex, REGEX_URL, REG_EXTENDED)) {
        return error_msg(REGEX_COMPILE_FAILED, is_testing);
    }

    if ((regexec(&regex, url, 0, NULL, 0)) == REG_NOMATCH) {
        return error_msg(OPT_URL_INVALID, is_testing);
    }

    regmatch_t matches[regex.re_nsub];

    if ((regexec(&regex, url, regex.re_nsub, matches, 0)) == REG_NOMATCH) {
        return error_msg(OPT_URL_INVALID, is_testing);
    }
    
    int urlPartIndex = -1;
    int urlLen = matches[0].rm_eo - matches[0].rm_so;
    regoff_t off; 
    regoff_t len;

    //printf("URL = \"%.*s\"\n", urlLen, url + matches[0].rm_so);
    for (int i = 1; i < regex.re_nsub; i++) {
        off = matches[i].rm_so;
        len = matches[i].rm_eo - matches[i].rm_so;

        if (len + off <= urlPartIndex) {
            continue;
        } else {
            urlPartIndex = off + len;

            char *tmp = (char*) malloc((len+1) * sizeof(char));
            
            int w = 0;
            for (int j = off; j < (len+off); j++) {
                tmp[w] = url[j];
                w++;
            }
            tmp[w]='\0';

            if ((strcmp("https", tmp)) == 0) {
                port = 443;
            } else if(tmp[0] == ':') {
                memmove(tmp, tmp+1, strlen(tmp));
                port = atoi(tmp);
            } else if (strstr(tmp, "www.")){
                hostname = (char*) malloc(strlen(tmp) * sizeof(char));
                strcpy(hostname, tmp);
            } else {
                hostname = (char*) malloc(strlen(tmp+4) * sizeof(char));
                strcpy(hostname, "www.");
                strcat(hostname, tmp);
            }

            free(tmp);
        }
    }
    
    off = urlPartIndex;
    len = urlLen - urlPartIndex;

    if (len) {
        path = (char*) malloc((urlLen+1) * sizeof(char));
            
        int w = 0;
        for (int j = off; j < (len+off); j++) {
            path[w] = url[j];
            w++;
        }
        path[w]='\0';
    } else {
        path = "/\0";
    }
    
    hosts[destinations_counter] = hostname;
    ports[destinations_counter] = port;
    paths[destinations_counter] = path;
    destinations_counter++;

    regfree(&regex);
    return 0;
}
