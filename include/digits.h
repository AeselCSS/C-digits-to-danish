#ifndef DIGITS_H
#define DIGITS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "digits_config.h"

// Function to convert a number to Danish words
void digit_to_danish(int num, char* buffer, struct digits_config config);

// Function to convert a number to Danish words considering configurations
void dansketal(int num, char* buffer, struct digits_config config);

// Function to run a test case
void run_test_case(int number, struct digits_config config, const char* expected);

#endif  // DIGITS_H
