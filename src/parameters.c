/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "parameters.h"

int parse_parameters (int argc, char *argv[], int is_testing) {
    extern char *optarg; 
    extern int optind;
    extern int opterr;
    extern int optopt;

    opterr = 0;
    optind = 1; //kvoli testom. Ked sa posebe vola getopt, optind ostáva po predchadzajucom pouziti

    int optFlags[7] = {0,0,0,0,0,0,0};
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
            regex_t regex;
            int regexResult;

            char urlRegex[] = "^https?:\\/\\/www\\.[a-zA-Z0-9]+([-\\.]{1}[a-zA-Z0-9]+)*\\.[a-z]{2,5}(:([1-9][0-9]{0,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5]))?(\\/.*)?$"; //"^https?:\/\/www\.[a-zA-Z0-9]+([-\.]{1}[a-zA-Z0-9]+)*\.[a-z]{2,5}(:[0-9]{1,5})?(\/.*)?$";

            regexResult = regcomp(&regex, urlRegex, REG_EXTENDED);
            if(regexResult) {
                return error_msg(REGEX_COMPILE_FAILED, is_testing);
            }

            regexResult = regexec(&regex, argv[optind], 0, NULL, 0);
            if (regexResult == REG_NOMATCH) {
                return error_msg(OPT_URL_INVALID, is_testing);
            }

            //skús vyparsovať port
        
            optFlags[DOMAIN_FLAG]++;
            optind++;
            regfree(&regex);
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