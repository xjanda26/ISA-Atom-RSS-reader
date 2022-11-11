/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/
#include <stdio.h>
#include <stdlib.h>

#include "feed.h"
#include "host.h"
#include "parameters.h"

int main (int argc, char *argv[]) {
    exit_value = 0;
    is_testing = 0;

    if (!parse_parameters(argc, argv)) {
        if (destinations_counter < 1) {
            return error_msg(TCP_NO_VALID_DEST);
        }

        for (int i = 0; i < destinations_counter; i++){
            get_and_print_feed(hosts[i], ports[i], paths[i], secure[i]);
        }
    }
    
    clear_destinations();
    return exit_value;
}
