/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "host.h"

char **hosts;
char **paths;
char **ports;

int destinations_counter = 0;
int *secure;

///TODO: dokumentacia
void init_destinations(int dest_count) {
    hosts = malloc(dest_count * sizeof(char*));
    paths = malloc(dest_count * sizeof(char*));
    ports = malloc(dest_count * sizeof(char*));
    secure = (int*) malloc(dest_count * sizeof(int));
}

///TODO: dokumentacia
void clear_destinations() {
    for (int i = 0; i < destinations_counter; i++) {
        if (hosts[i]) {
            free(hosts[i]);
        }
        if (paths[i]) {
            free(paths[i]);
        }
        if (ports[i]) {
            free(ports[i]);
        }
    }

    if (hosts) {
        free(hosts);
    }
    if (paths) {
        free(paths);
    }
    if (ports) {
        free(ports);
    }
    if (secure) {
        free(secure);
    }
}

/**
 * Regex inspiration: https://man7.org/linux/man-pages/man3/regexec.3.html
 * //TODO: dokumentacia
*/
int parse_url (char *url, int is_getting_data) {
    regex_t regex;

    char *port;
    char *hostname;
    char *path;

    if(regcomp(&regex, REGEX_URL, REG_EXTENDED)) {
        return error_msg(REGEX_COMPILE_FAILED);
    }

    if ((regexec(&regex, url, 0, NULL, 0)) == REG_NOMATCH) {
        return error_msg(OPT_URL_INVALID);
    }

    regmatch_t matches[regex.re_nsub];

    if ((regexec(&regex, url, regex.re_nsub, matches, 0)) == REG_NOMATCH) {
        return error_msg(OPT_URL_INVALID);
    }
    
    if (is_getting_data) {
        int urlPartIndex = -1;
        int urlLen = matches[0].rm_eo - matches[0].rm_so;
        int isSecure = 0;
        regoff_t off; 
        regoff_t len;

        port = (char*) malloc(6 * sizeof(char));

        strcpy(port, "80");

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

                tmp[w] = '\0';

                int tmp_len = strlen(tmp);
                if ((strcmp("https", tmp)) == 0) {
                    isSecure = 1;
                    strcpy(port, "443");
                } else if(tmp[0] == ':') {
                    memmove(tmp, tmp + 1, tmp_len);
                    strcpy(port, tmp);
                } else if (strstr(tmp, "www.")){ 
                    ///TODO: nepridávat www. (vyjadril sa k tomu Polcak)
                    hostname = (char*) malloc((tmp_len + 1) * sizeof(char));
                    strcpy(hostname, tmp);
                } else {
                    hostname = (char*) malloc((tmp_len + 5) * sizeof(char));
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
        secure[destinations_counter] = isSecure;
        destinations_counter++;
    }

    regfree(&regex);
    return SUCCESS;
}

///TODO: dokumentacia
int read_urls(FILE *fp, int is_getting_data) {
    char *line;
    size_t len = 0;

    int urlCount = 0;
    while ((getline(&line, &len, fp)) != -1) {
        if(strlen(line) > 1) {
            if (line[0] != '#') {
                if(!parse_url(line,is_getting_data)){
                    urlCount++;
                }
            }            
        }        
    }

    if (line) {
        free(line);
    }

    return urlCount;
}

///TODO: dokumentacia
int parse_url_from_file(char *file_name) {
    //printf("File reading\n");

    FILE *fp;
    int urlCount = 0;

    fp = fopen(file_name, "r");

    if(fp == NULL) {
        return error_msg(FILE_READING_FAILED);
    }

    urlCount = read_urls(fp, 0);

    //printf("Number of corrent URLs: %i\n", urlCount);

    if (urlCount) {
        fp = fopen(file_name, "r");

        if(fp == NULL) {
            return error_msg(FILE_READING_FAILED);
        }

        init_destinations(urlCount);
        read_urls(fp, 1);

        fclose(fp);
    } else {
        return error_msg(FILE_EMPTY_INVALID_URLS);
    }

    return SUCCESS;
}
