/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "parameters.h"

int parse_parameters (int argc, char *argv[], int is_testing) {
    extern char *optarg; 
    extern int optind;
    extern int opterr;

    opterr = 0;

    int optFlags[6] = {0,0,0,0,0,0};
    int opt, i, optErrFlag = 0;
    
    if (argc < 2) {
        return error_msg(OPT_FEW, is_testing);
    }

    while ((opt = getopt(argc, argv, "ac:C:f:Tu")) != -1) {
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
                optErrFlag = 1;
        }
    }

    if (optErrFlag) {
        return error_msg(OPT_UNKNOWN, is_testing);
    }

    for (i = 0; i < 6; i++) {
        if (optFlags[i] > 1) {
            return error_msg(OPT_MULTIPLE, is_testing);
        }
    }

    if (optFlags[C_FLAG] == 1 && optFlags[CC_FLAG] == 1) {
        return error_msg(OPT_MUL_COMBINATION, is_testing);
    }

    if (optind > 0 && optFlags[F_FLAG] < 0) {
        return error_msg(OPT_MUL_DOMAINS, is_testing);
    }

    printf("OPTIND: %i\n", optind);
    for (i = optind; i < argc; i++) {
        printf ("Non-option argument %s\n", argv[i]);
    }
    
    return 0;
}