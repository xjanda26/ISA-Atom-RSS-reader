/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "parameter_tests.h"

int test_TP1() {
    char *in_var[] = {"./feedreader"};
    int result, argc = 1;

    result = test(ERR_OPT_FEW, parse_parameters(argc, in_var, TRUE), "TP1");

    return result;
}

int test_TP2() {
    char *in_var[] = {"./feedreader", "https://www.fit.vut.cz/fit/news-rss/", "-f", "files/test.txt"};
    int argc = 4;

    return test(ERR_OPT_MUL_DOMAINS, parse_parameters(argc, in_var, TRUE), "TP2");
}

int test_TP3() {
    char *in_var[] = {"./feedreader", "https://www.fit.vut.cz/fit/news-rss/"};
    int argc = 2;

    return test(0, parse_parameters(argc,in_var, TRUE), "TP3");
}

int test_TP4() {
    char *in_var[] = {"./feedreader", "http://www.fit.vut.cz/fit/news-rss/"};
    int argc = 2;

    return test(0, parse_parameters(argc,in_var, TRUE), "TP4");
}

int test_TP5() {
    char *in_var[] = {"./feedreader", "http://fit.vut.cz/fit/news-rss/"};
    int argc = 2;

    return test(0,parse_parameters(argc,in_var, TRUE), "TP5");
}

int test_TP6() {
    char *in_var[] = {"./feedreader", "http://.fit.vut.cz/fit/news-rss/"};
    int argc = 2;

    return test(ERR_OPT_URL_INVALID,parse_parameters(argc,in_var, TRUE), "TP6");
}

int test_TP7() {
    char *in_var[] = {"./feedreader", "http://www.fit.vut.cz:80/fit/news-rss/"};
    int argc = 2;

    return test(0, parse_parameters(argc,in_var, TRUE), "TP7");
}

int test_TP8() {
    char *in_var[] = {"./feedreader", "http://www.fit.vut.cz:65535/fit/news-rss/"};
    int argc = 2;

    return test(0, parse_parameters(argc,in_var, TRUE), "TP8");
}

int test_TP9() {
    char *in_var[] = {"./feedreader", "http://www.fit.vut.cz:65536/fit/news-rss/"};
    int argc = 2;

    return test(ERR_OPT_URL_INVALID, parse_parameters(argc,in_var, TRUE), "TP9");
}

int test_TP10() {
    char *in_var[] = {"./feedreader", "www.fit.vut.cz/fit/news-rss/"};
    int argc = 2;

    return test(ERR_OPT_URL_INVALID, parse_parameters(argc,in_var, TRUE), "TP10");
}

int test_TP11() {
    char *in_var[] = {"./feedreader", "http://www.fit.vut.cz/fit/news-rss/", "-a"};
    int argc = 3;

    return test(0,parse_parameters(argc,in_var, TRUE), "TP11");
}

int test_TP12() {
    char *in_var[] = {"./feedreader", "-u", "http://www.fit.vut.cz/fit/news-rss/", "-a"};
    int argc = 4;

    return test(0,parse_parameters(argc,in_var, TRUE), "TP12");
}

int test_TP13() {
    char *in_var[] = {"./feedreader", "-T", "http://www.fit.vut.cz/fit/news-rss/", "-a"};
    int argc = 4;

    return test(0,parse_parameters(argc,in_var, TRUE), "TP13");
}

/*
int test_TP() {
    char *in_var[] = {};
    int argc =;

    return test(,parse_parameters(argc,in_var, TRUE), "TP");
}
*/

void run_parameter_tests(){
    int counter = 4;
    int number_of_passed = 0;

    printf("===- Parameter testing -===\n");
    
    number_of_passed += test_TP1();
    number_of_passed += test_TP2();
    number_of_passed += test_TP3();
    number_of_passed += test_TP4();
    /*number_of_passed += test_TP5();
    number_of_passed += test_TP6();
    number_of_passed += test_TP7();
    number_of_passed += test_TP8();
    number_of_passed += test_TP9();
    number_of_passed += test_TP10();
    number_of_passed += test_TP11();
    number_of_passed += test_TP12();
    number_of_passed += test_TP13();*/
    
    int failed = counter - number_of_passed;
    if (failed < 0) {
        failed = 0;
    }

    printf("\nParameter testing done.\nNumber of all: %d\tpassed: %d\tfailed: %d\n\n", counter, number_of_passed, failed);
}
