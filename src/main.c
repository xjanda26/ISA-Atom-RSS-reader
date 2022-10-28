/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/
#include <stdio.h>

#include "connectionAri.h"
#include "host.h"
#include "parameters.h"

int main (int argc, char *argv[]) {
    int result = 0;

    result = parse_parameters(argc, argv, 0);
    printf("File flag: %i\n", optFlags[F_FLAG]);
    //printf("Host: %s\n", hosts[0].hostName);
    printf("File path: %s\n", filePath);

    if (!result) {
        connectRRS();
    }
    
    return result;
}
