/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/
#include <stdio.h>

#include "feed.h"
#include "host.h"
#include "parameters.h"

int main (int argc, char *argv[]) {
    int result = 0;

    init_destinations();
    result = parse_parameters(argc, argv, 0);

    if (!result) {
        printf("Trying connect\n");
        for (int i = 0; i < destinations_counter; i++){
            printf("Port %i\n", ports[i]);
            printf("Hostname: %s\n", hosts[i]);
            printf("Path: %s\n", paths[i]);
        }

        //get_and_print_feed(hosts[0]);
    }
    
    clear_destinations();
    return result;
}
