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
        printf("Trying to connect to %i dest\n", destinations_counter);
        for (int i = 0; i < destinations_counter; i++){
            printf("Po: %i ", ports[i]);
            printf(" H: %s ", hosts[i]);
            printf("Pa: %s\n", paths[i]);
        }

        //get_and_print_feed(hosts[0]);
    }
    
    clear_destinations();
    return result;
}
