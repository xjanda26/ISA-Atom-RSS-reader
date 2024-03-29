/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/
///TODO: vsetko
#include "parameter_tests.h"

int test_TP1() {
    char *in_var[] = {"./feedreader"};

    return test(ERR_OPT_FEW, parse_parameters(ARGV_LEN(in_var), in_var), "TP1");;
}

int test_TP2() {
    char *in_var[] = {"./feedreader", "https://www.fit.vut.cz/fit/news-rss/", "-f", "tests/examples/test.txt"};

    return test(ERR_OPT_MUL_DOMAINS, parse_parameters(ARGV_LEN(in_var), in_var), "TP2");
}

int test_TP3() {
    char *in_var[] = {"./feedreader", "https://www.fit.vut.cz/fit/news-rss/"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP3");
}

int test_TP4() {
    char *in_var[] = {"./feedreader", "http://www.fit.vut.cz/fit/news-rss/"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP4");
}

int test_TP5() {
    char *in_var[] = {"./feedreader", "http://fit.vut.cz/fit/news-rss/"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP5");
}

int test_TP6() {
    char *in_var[] = {"./feedreader", "http://.fit.vut.cz/fit/news-rss/"};

    return test(ERR_OPT_URL_INVALID, parse_parameters(ARGV_LEN(in_var), in_var), "TP6");
}

int test_TP7() {
    char *in_var[] = {"./feedreader", "https://www.fit.vut.cz:80/fit/news-rss/"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP7");
}

int test_TP8() {
    char *in_var[] = {"./feedreader", "https://www.fit.vut.cz:65535/fit/news-rss/"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP8");
}

int test_TP9() {
    char *in_var[] = {"./feedreader", "http://www.fit.vut.cz:65536/fit/news-rss/"};

    return test(ERR_OPT_URL_INVALID, parse_parameters(ARGV_LEN(in_var), in_var), "TP9");
}

int test_TP10() {
    char *in_var[] = {"./feedreader", "www.fit.vut.cz/fit/news-rss/"};

    return test(ERR_OPT_URL_INVALID, parse_parameters(ARGV_LEN(in_var), in_var), "TP10");
}

int test_TP11() {
    char *in_var[] = {"./feedreader", "https://www.fit.vut.cz/fit/news-rss/", "-a"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP11");
}

int test_TP12() {
    char *in_var[] = {"./feedreader", "-u", "https://www.fit.vut.cz/fit/news-rss/", "-a"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP12");
}

int test_TP13() {
    char *in_var[] = {"./feedreader", "-T", "https://www.fit.vut.cz/fit/news-rss/", "-a"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP13");
}

int test_TP14() {
    char *in_var[] = {"./feedreader", "-T", "-a", "https://www.fit.vut.cz/fit/news-rss/"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP14");
}

int test_TP15() {
    char *in_var[] = {"./feedreader", "https://www.fit.vut.cz/fit/news-rss/", "-T", "-a"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP15");
}

int test_TP16() {
    char *in_var[] = {"./feedreader", "https://www.fit.vut.cz/fit/news-rss/", "-T", "-a", "-u"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP16");
}

int test_TP17() {
    char *in_var[] = {"./feedreader", "-T", "-a", "-u", "https://www.fit.vut.cz/fit/news-rss/"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP17");
}

int test_TP18() {
    char *in_var[] = {"./feedreader", "-c", "-a", "https://www.fit.vut.cz/fit/news-rss/"};

    return test(ERR_OPT_CERT_PATH_MISSING, parse_parameters(ARGV_LEN(in_var), in_var), "TP18");
}

int test_TP19() {
    char *in_var[] = {"./feedreader", "-a", "https://www.fit.vut.cz/fit/news-rss/", "-c"};

    return test(ERR_OPT_CERT_PATH_MISSING, parse_parameters(ARGV_LEN(in_var), in_var), "TP19");
}

int test_TP20() {
    char *in_var[] = {"./feedreader", "-a", "https://www.fit.vut.cz/fit/news-rss/", "-c", "cert.pem"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP20");
}

int test_TP21() {
    char *in_var[] = {"./feedreader", "-C", "-a", "-T", "https://www.fit.vut.cz/fit/news-rss/"};

    return test(ERR_OPT_FOLDER_PATH_MISSING, parse_parameters(ARGV_LEN(in_var), in_var), "TP21");
}

int test_TP22() {
    char *in_var[] = {"./feedreader", "-a", "https://www.fit.vut.cz/fit/news-rss/", "-T", "-C"};

    return test(ERR_OPT_FOLDER_PATH_MISSING, parse_parameters(ARGV_LEN(in_var), in_var), "TP22");
}

int test_TP23() {
    char *in_var[] = {"./feedreader", "-a", "https://www.fit.vut.cz/fit/news-rss/", "-C", "tests"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP23");
}

int test_TP24() {
    char *in_var[] = {"./feedreader", "-a", "https://www.fit.vut.cz/fit/news-rss/", "-C", "-u", "tests"};

    return test(ERR_OPT_FOLDER_PATH_MISSING, parse_parameters(ARGV_LEN(in_var), in_var), "TP24");
}

int test_TP25() {
    char *in_var[] = {"./feedreader", "-f", "tests/examples/hosts.txt", "-T", "-a", "-u"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP25");
}

int test_TP26() {
    char *in_var[] = {"./feedreader", "-T", "-a", "-u", "-f", "tests/examples/hosts.txt"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP26");
}

int test_TP27() {
    char *in_var[] = {"./feedreader", "-c", "-a", "-f", "tests/examples/hosts.txt"};

    return test(ERR_OPT_CERT_PATH_MISSING, parse_parameters(ARGV_LEN(in_var), in_var), "TP27");
}

int test_TP28() {
    char *in_var[] = {"./feedreader", "-a", "-f", "tests/examples/hosts.txt", "-c"};

    return test(ERR_OPT_CERT_PATH_MISSING, parse_parameters(ARGV_LEN(in_var), in_var), "TP28");
}

int test_TP29() {
    char *in_var[] = {"./feedreader", "-a", "-f", "tests/examples/hosts.txt", "-c", "cert.pem"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP29");
}

int test_TP30() {
    char *in_var[] = {"./feedreader", "-C", "-a", "-T", "-f", "tests/examples/hosts.txt"};

    return test(ERR_OPT_FOLDER_PATH_MISSING, parse_parameters(ARGV_LEN(in_var), in_var), "TP30");
}

int test_TP31() {
    char *in_var[] = {"./feedreader", "-a", "-f", "tests/examples/hosts.txt", "-T", "-C"};

    return test(ERR_OPT_FOLDER_PATH_MISSING, parse_parameters(ARGV_LEN(in_var), in_var), "TP31");
}

int test_TP32() {
    char *in_var[] = {"./feedreader", "-a", "-f", "tests/examples/hosts.txt", "-C", "tests"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP32");
}

int test_TP33() {
    char *in_var[] = {"./feedreader", "-a", "-f", "hosts.txt", "-C", "-u", "tests"};

    return test(ERR_OPT_FOLDER_PATH_MISSING, parse_parameters(ARGV_LEN(in_var), in_var), "TP33");
}

int test_TP34() {
    char *in_var[] = {"./feedreader", "-u", "-c", "tests/examples/FIT-cert", "https://www.fit.vut.cz/fit/news-rss/"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP34");
}

int test_TP35() {
    char *in_var[] = {"./feedreader", "-a", "-c", "tests/examples/FIT-cert", "-f", "tests/examples/hosts.txt"};

    return test(SUCCESS, parse_parameters(ARGV_LEN(in_var), in_var), "TP35");
}

/* Copy-pasta template
int test_TP() {
    char *in_var[] = {};

    return test(, parse_parameters(ARGV_LEN(in_var),in_var, TRUE), "TP");
}
*/

void run_parameter_tests(){
    int counter = 35;
    int number_of_passed = 0;

    if (LANG) {
        printf("===- Testování vstupních argumentů -===\n");
    } else {
        printf("===- Parameter testing -===\n");
    }
    
    number_of_passed += test_TP1();
    number_of_passed += test_TP2();
    number_of_passed += test_TP3();
    number_of_passed += test_TP4();
    number_of_passed += test_TP5();
    number_of_passed += test_TP6();
    number_of_passed += test_TP7();
    number_of_passed += test_TP8();
    number_of_passed += test_TP9();
    number_of_passed += test_TP10();
    number_of_passed += test_TP11();
    number_of_passed += test_TP12();
    number_of_passed += test_TP13();
    number_of_passed += test_TP14();
    number_of_passed += test_TP15();
    number_of_passed += test_TP16();
    number_of_passed += test_TP17();
    number_of_passed += test_TP18();
    number_of_passed += test_TP19();
    number_of_passed += test_TP20();
    number_of_passed += test_TP21();
    number_of_passed += test_TP22();
    number_of_passed += test_TP23();
    number_of_passed += test_TP24();
    number_of_passed += test_TP25();
    number_of_passed += test_TP26();
    number_of_passed += test_TP27();
    number_of_passed += test_TP28();
    number_of_passed += test_TP29();
    number_of_passed += test_TP30();
    number_of_passed += test_TP31();
    number_of_passed += test_TP32();
    number_of_passed += test_TP33();
    number_of_passed += test_TP34();
    number_of_passed += test_TP35();
    
    int failed = counter - number_of_passed;
    if (failed < 0) {
        failed = 0;
    }

    if (LANG) {
        printf("\nTestování vstupních argumentů dokončeno.\nPočet všech: %d\tprošlo: %d\tneúspěšné: %d\n\n", counter, number_of_passed, failed);
    } else {
        printf("\nParameter testing done.\nNumber of all: %d\tpassed: %d\tfailed: %d\n\n", counter, number_of_passed, failed);
    }
    
}
