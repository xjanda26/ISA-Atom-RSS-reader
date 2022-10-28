/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "parameters.h"

int optFlags[7];
char *hostName;
char *port;
char *filePath;

void init_parameter_variables() {
    for (int i = 0; i < 7; i++) {
        optFlags[i] = 0;
    }

    hostName = NULL;
    port = NULL;
    filePath = NULL;
}

int parse_parameters (int argc, char *argv[], int is_testing) {
    extern char *optarg; 
    extern int optind;
    extern int opterr;
    extern int optopt;

    opterr = 0;
    optind = 1; //kvoli testom. Ked sa posebe vola getopt, optind ostáva po predchadzajucom pouziti
    init_parameter_variables();

    int opt, i, optErrFlag = 0, optPathMissFlag = 0;
    
    if (argc < 2) {
        return error_msg(OPT_FEW, is_testing);
    }

    while (optind < argc) {
        if ((opt = getopt(argc, argv, "ac:C:f:Tu")) != -1) {
            switch(opt) {
                case 'a':
                    optFlags[A_FLAG]++;
                    break;
                case 'c':
                    optFlags[C_FLAG]++;
                    break;
                case 'C':
                    optFlags[CC_FLAG]++;
                    break;
                case 'f':
                    optFlags[F_FLAG]++;
                    filePath = optarg;
                    break;
                case 'T':
                    optFlags[T_FLAG]++;
                    break;
                case 'u':
                    optFlags[U_FLAG]++;
                    break;
                case '?':
                    if (optopt == 'f') {
                        optPathMissFlag = 1;
                    } else {
                        optErrFlag = 1;
                    }
            }
        } else {
            int urlParsingRes = parse_url(argv[optind], is_testing);

            if(urlParsingRes) {
                return urlParsingRes;
            }

            optFlags[DOMAIN_FLAG]++;
            optind++;
        }
    }

    if (optErrFlag) {
        return error_msg(OPT_UNKNOWN, is_testing);
    }

    if (optPathMissFlag) {
        return error_msg(OPT_PATH_MISSING, is_testing);
    }

    for (i = 0; i < 6; i++) {
        if (optFlags[i] > 1) {
            return error_msg(OPT_MULTIPLE, is_testing);
        }
    }

    if (optFlags[C_FLAG] > 0 && optFlags[CC_FLAG] > 0) {
        return error_msg(OPT_MUL_COMBINATION, is_testing);
    }

    if (optFlags[DOMAIN_FLAG] > 0 && optFlags[F_FLAG] > 0) {
        return error_msg(OPT_MUL_DOMAINS, is_testing);
    }
    
    return 0;
}