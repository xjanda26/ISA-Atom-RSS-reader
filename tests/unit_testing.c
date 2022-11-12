/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "unit_testing.h"

int test(int expected, int actual, const char* testCaseID) {
    int passed = 0;

    if (expected == actual) {
        passed = 1;
    } else {
        if (LANG) {
            printf("%s NEÚSPĚCH, očekávano: %d\taktuální: %d\n", testCaseID, expected, actual);
        } else {
            printf("%s FAILED, expected: %d\tactual: %d\n", testCaseID, expected, actual);
        }
    }

    return passed;
}
