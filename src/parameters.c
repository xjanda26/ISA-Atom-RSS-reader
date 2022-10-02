/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "parameters.h"

int parse_parameters (int argc, char *argv[], int is_testing) {
    if (argc < 1) {
        if(!is_testing)
            fprintf(stderr, "Error. No argument on STDIN.\n");
        
        return 1;
    }
    return 0;
}