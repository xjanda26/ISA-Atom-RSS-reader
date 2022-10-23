/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "error.h"
#include "parameter_tests.h"

int test_TP1() {
    char *in_var = "testtest";

    return test(ERR_OPT_FEW, parse_parameters(1, (char **)in_var, TRUE), "TP1");
}

int test_TP2() {
    //char *in_var = (char*) malloc(69*sizeof(char));

    //in_var = "./feedreader https://www.fit.vut.cz/fit/news-rss/ -f files/test.txt";

    char in_var[] = "./feedreader https://www.fit.vut.cz/fit/news-rss/ -f files/test.txt";

    int result = test(ERR_OPT_MUL_DOMAINS, parse_parameters(4, (char **)in_var, TRUE), "TP2");

    //free(in_var);
    return result;
}

void run_parameter_tests(){
    int counter = 2;
    int number_of_passed = 0;

    printf("===- Parameter testing -===\n");
    
    number_of_passed += test_TP1();
    number_of_passed += test_TP2();
    
    printf("\nParameter testing done.\nNumber of all: %d\tpassed: %d\tfailed: %d\n\n", counter, number_of_passed, (counter - number_of_passed));
}
