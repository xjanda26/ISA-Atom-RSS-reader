/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "parameter_tests.h"

int test_TP1() {
    char *in_var = "testtest";
    return test(1, parse_parameters(1, (char**)in_var, 1), "TP1");
}

void run_parameter_tests(){
    int counter = 1;
    int number_of_passed = 0;

    printf("===- Parameter testing -===\n");
    
    number_of_passed += test_TP1();
    
    printf("\nParameter testing done.\nNumber of all: %d\tpassed: %d\tfailed: %d\n\n", counter, number_of_passed, (counter - number_of_passed));
}
