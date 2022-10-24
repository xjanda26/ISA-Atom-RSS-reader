/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/
#include <stdio.h>

#include "connectionAri.h"
#include "parameters.h"

int main (int argc, char *argv[]) {
    int result = 0;

    result = parse_parameters(argc, argv, 0);
    if (!result) {
        connectRRS();
    }
    
    return result;
}