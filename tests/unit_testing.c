/**
 * Author: Adam Janda, xjanda26@stud.fit.vutbr.cz
*/

#include "unit_testing.h"

int test(int expected, int actual, const char* testCaseID) {
    int passed = 0;

    if (expected == actual) {
        //printf("%s PASSED\n", testCaseID);
        passed = 1;
    } else {
        printf("%s FAILED, expected: %d\tactual: %d\n", testCaseID, expected, actual);
    }

    return passed;
}
