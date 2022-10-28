/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/
#include <stdio.h>

#include "feed.h"
#include "host.h"
#include "parameters.h"

int main (int argc, char *argv[]) {
    int result = 0;

    result = parse_parameters(argc, argv, 0);

    if (!result) {
        get_and_print_feed(hosts[0]);
    }
    
    free(hosts[0]);
    return result;
}
