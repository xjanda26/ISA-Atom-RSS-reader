/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
 */

#include "host.h"

char **hosts;
char **paths;
char **ports;

int destinations_counter = 0;
int *secure;

/// @brief Function allocates application array with length defiend by dest_count.
/// @param dest_count Number of remote hosts
void init_destinations(int dest_count) {
    destinations_counter = 0;
    hosts = malloc(dest_count * sizeof(char *));
    paths = malloc(dest_count * sizeof(char *));
    ports = malloc(dest_count * sizeof(char *));
    secure = (int *) malloc(dest_count * sizeof(int));
}

/// @brief Function deallocates reserved memory.
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

    hosts = NULL;
    paths = NULL;
    ports = NULL;
    secure = NULL;
}

/// @brief Function checks entered URL and saves its metadata for later use.
///        Function works in 2 modes controlled by @param is_getting_data.
///        If @param is_getting_data is zero then function just checks URL syntax.
///        If @param is_getting_data is bigger than zero, function checks URL and 
///        saves data for later use.
///        https://man7.org/linux/man-pages/man3/regexec.3.html
/// @param url Checked URL
/// @param is_getting_data Mode flag
/// @return Zero on success or regex or url error types defined in error.h
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

        port = (char *) malloc(6 * sizeof(char));

        strcpy(port, "80");

        for (int i = 1; i < regex.re_nsub; i++) {
            off = matches[i].rm_so;
            len = matches[i].rm_eo - matches[i].rm_so;

            if (len + off <= urlPartIndex) {
                continue;
            }
            
            urlPartIndex = off + len;

            char *tmp = (char *) malloc((len + 1) * sizeof(char));
            
            int w = 0;
            for (int j = off; j < (len + off); w++, j++) {
                tmp[w] = url[j];
            }

            tmp[w] = '\0';

            int tmp_len = strlen(tmp);
            if ((strcmp("https", tmp)) == 0) {
                isSecure = 1;
                strcpy(port, "443");
            } else if(tmp[0] == ':') {
                memmove(tmp, tmp + 1, tmp_len);
                strcpy(port, tmp);
            } else { 
                hostname = (char *) malloc((tmp_len + 1) * sizeof(char));
                strcpy(hostname, tmp);
            }

            free(tmp);
        }
    
        off = urlPartIndex;
        len = urlLen - urlPartIndex;

        if (len) {
            path = (char *) malloc((urlLen + 1) * sizeof(char));

            int w = 0;
            for (int j = off; j < (len + off); w++, j++) {
                path[w] = url[j];
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

/// @brief Function reads file for URLs, ignoring lines started with '#' (comment).
///        Function works in 2 modes controlled by is_getting_data.
///        If is_getting_data is zero then function just checks URL syntax.
///        If is_getting_data is bigger than zero, function checks URL and 
///        saves data for later use.
/// @param fp File stream
/// @param is_getting_data Mode flag
/// @return Number of valid URLs stored in file
int read_urls(FILE *fp, int is_getting_data) {
    char *line;
    size_t len = 0;

    int urlCount = 0;
    while ((getline(&line, &len, fp)) != -1) {
        if(strlen(line) > 1 && line[0] != '#' && !parse_url(line,is_getting_data)) {
            urlCount++;            
        }        
    }

    if (line) {
        free(line);
    }

    return urlCount;
}

/// @brief Function reads file and validates its contain. 
///        On any valid URL format, function creates metadata
///        for later communication.
/// @param file_name Path to the file
/// @return Zero on success or any FILE error type defined in error.h
int parse_url_from_file(char *file_name) {
    FILE *fp;
    int urlCount = 0;

    fp = fopen(file_name, "r");

    if(fp == NULL) {
        return error_msg(FILE_READING_FAILED);
    }

    urlCount = read_urls(fp, 0);

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
