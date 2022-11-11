/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "error.h"
#include "parameter_tests.h"

void run_test() {
    run_parameter_tests();
}

int main (void) {
    exit_value = 0;
    is_testing = 1;
    run_test();
    return 0;
}
