/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/
#include <stdio.h>
#include <stdlib.h>

#include "feed.h"
#include "host.h"
#include "parameters.h"

/// @brief Main function of application. Usage is described in README file.
/// @param argc Number of entered arguments
/// @param argv String of entered arguments
/// @return Zero value on success or any error type defined in error.h
int main (int argc, char *argv[]) {
    // Initializing global flags
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
