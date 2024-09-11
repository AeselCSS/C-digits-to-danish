#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "digits.h"

void run_test_case(int number, struct digits_config config, const char* expected) {
    char buffer[256];
    memset(buffer, 0, sizeof(buffer));
    
    dansketal(number, buffer, config);
    
    printf("\n----------------------------------------\n");
    printf("Input: %d, Output: %s, Expected: %s\n", number, buffer, expected);
    if (strcmp(buffer, expected) == 0) {
        printf("\nTEST PASSED!\n");
        printf("----------------------------------------\n");
    } else {
        printf("\nTEST FAILED!\n");
        printf("----------------------------------------\n");
    }
}

