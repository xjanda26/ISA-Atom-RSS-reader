/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "parameters.h"

int optFlags[7];
char *filePath;

void init_parameter_variables() {
    for (int i = 0; i < 7; i++) {
        optFlags[i] = 0;
    }
}

int parse_hostname(char *argv[], int i, int is_testing) {
    //printf("rozne\n");
    
    init_destinations(1);
    int urlParsingRes = parse_url(argv[i], 1, is_testing);

    if(urlParsingRes) {
        return urlParsingRes;
    }

    optFlags[DOMAIN_FLAG]++;
    return 0;
}

int parse_parameters (int argc, char *argv[], int is_testing) {
    extern char *optarg; 
    extern int optind;
    extern int opterr;
    extern int optopt;

    opterr = 0;
    optind = 1; //kvoli testom. Ked sa posebe vola getopt, optind ostáva po predchadzajucom pouziti
    init_parameter_variables();

    int opt, res, i = 2, optErrFlag = 0, optPathMissFlag = 0;
    
    if (argc < 2) {
        return error_msg(OPT_FEW, is_testing);
    }

    while (optind < argc) {
        //printf("ARGC: %i\n", argc);
        //printf("OPTIND start %i\n==\n\n", optind);

        opt = getopt(argc, argv, "ac:C:f:Tu");
        //printf("OPT case: %c\n", opt);
        //printf("OPTIND %i, i:%i\n", optind, i);

        if ((opt != -1 && optind == i) || opt == 'f') {
            //printf("switch-case\n");
            i++;
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
            if (optFlags[DOMAIN_FLAG] > 1) {
                return error_msg(OPT_MULTIPLE, is_testing);
            }
            res = parse_hostname(argv, i-1, is_testing);
            
            if (res) {
                return res;
            }

            optFlags[DOMAIN_FLAG]++;

            if (i == argc) {
                optind++;
            } else {
                optind--;
                i++;
            }
            
        }
        
        //printf("next itera ARGC:%i,OPTIND:%i\n", argc, optind);
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
        clear_destinations();
        return error_msg(OPT_MUL_DOMAINS, is_testing);
    }

    if (optFlags[DOMAIN_FLAG] == 0 && optFlags[F_FLAG] == 0) {
        return error_msg(OPT_NO_HOST, is_testing);
    }
    
    if (optFlags[F_FLAG]) {
        int fileReadingRes = parse_url_from_file(filePath, is_testing);
        if (fileReadingRes) {
            return fileReadingRes;
        }
    }

    return 0;
}