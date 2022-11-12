/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "parameters.h"

int optFlags[7];
char *filePath;
char *certPath;
char *certFolder;

/// @brief Function initializes array of application flags.
void init_parameter_variables() {
    for (int i = 0; i < 7; i++) {
        optFlags[i] = 0;
    }
}

/// @brief Function initializes and parse remote host name.
/// @param argv String of entered arguments
/// @param i Hostname position
/// @return Zero value on success or OPT, REGEX or FILE error type defined in error.h
int parse_hostname(char *argv[], int i) {
    init_destinations(1);

    if(parse_url(argv[i], 1)) {
        return exit_value;
    }

    optFlags[DOMAIN_FLAG]++;
    return SUCCESS;
}

/// @brief Function parses entered arguments by user.
/// @param argc Number of entered arguments
/// @param argv String of entered arguments
/// @return Zero value on success or OPT, REGEX or FILE error type defined in error.h
int parse_parameters (int argc, char *argv[]) {
    extern char *optarg; 
    extern int optind;
    extern int opterr;
    extern int optopt;

    opterr = 0;
    optind = 1; //kvoli testom. Ked sa posebe vola getopt, optind ostáva po predchadzajucom pouziti
    init_parameter_variables();

    int opt, res, i = 2;
    int optErrFlag = 0, optPathMissFlag = 0;
    int optCertFileMissFlag = 0, optCertFolderMissFlag = 0;
    
    if (argc < 2) {
        return error_msg(OPT_FEW);
    }

    while (i - 1 < argc) {
        opt = getopt(argc, argv, "ac:C:f:Tu");

        if (opt == 'c' || opt == 'C' || opt == 'f') {
            optind --;
        }

        if (i != optind) {
            if (optFlags[DOMAIN_FLAG] > 0 ) {
                if (optFlags[F_FLAG] > 0) {
                    return error_msg(OPT_MUL_DOMAINS);
                }
                
                return error_msg(OPT_MULTIPLE);
            }
            res = parse_hostname(argv, i-1);
            
            if (res) {
                return res;
            }

            optFlags[DOMAIN_FLAG]++;

            if (i == argc) {
                break;
            }

            i++;
        }

        switch(opt) {
            case 'a':
                optFlags[A_FLAG]++;
                break;
            case 'c':
                optFlags[C_FLAG]++;
                certPath = optarg;
                if (certPath[0] == '-') {
                    optCertFileMissFlag = 1;
                }

                if (optind + 1 == argc) {
                    i++;
                }
                break;
            case 'C':
                optFlags[CC_FLAG]++;
                certFolder = optarg;
                if (certFolder[0] == '-') {
                    optCertFolderMissFlag = 1;
                }

                if (optind + 1 == argc) {
                    i++;
                }
                break;
            case 'f':
                optFlags[F_FLAG]++;
                filePath = optarg;
                if (filePath[0] == '-') {
                    optPathMissFlag = 1;
                }

                if (optind + 1 == argc) {
                    i++;
                }
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
                } else if (optopt == 'c') {
                    optCertFileMissFlag = 1;
                } else if (optopt == 'C') {
                    optCertFolderMissFlag = 1;
                } else {
                    optErrFlag = 1;
                }
        }
        i++;
    }

    if (optErrFlag) {
        return error_msg(OPT_UNKNOWN);
    }

    if (optCertFileMissFlag) {
        return error_msg(OPT_CERT_PATH_MISSING);
    }
    
    if (optCertFolderMissFlag) {
        return error_msg(OPT_FOLDER_PATH_MISSING);
    }

    if (optPathMissFlag) {
        return error_msg(OPT_PATH_MISSING);
    }

    for (i = 0; i < 6; i++) {
        if (optFlags[i] > 1) {
            return error_msg(OPT_MULTIPLE);
        }
    }

    if (optFlags[C_FLAG] > 0 && optFlags[CC_FLAG] > 0) {
        return error_msg(OPT_MUL_COMBINATION);
    }

    if (optFlags[DOMAIN_FLAG] > 0 && optFlags[F_FLAG] > 0) {
        return error_msg(OPT_MUL_DOMAINS);
    }

    if (optFlags[DOMAIN_FLAG] == 0 && optFlags[F_FLAG] == 0) {
        return error_msg(OPT_NO_HOST);
    }
    
    if (optFlags[F_FLAG]) {
        if (parse_url_from_file(filePath)) {
            return exit_value;
        }
    }

    return SUCCESS;
}
