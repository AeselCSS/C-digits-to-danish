#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Struct for configuration options
struct digits_config {
    int use_plural;  // If 1, use plural forms where applicable
    int use_og;      // If 1, include "og" between tens and ones
    int skip_one;    // If 1, skip "et" in front of hundreds or thousands
};

void digit_to_danish(int num, char* buffer, struct digits_config config) {
    const char* ones[] = {"", "et", "to", "tre", "fire", "fem", "seks", "syv", "otte", "ni"};
    const char* teens[] = {"ti", "elleve", "tolv", "tretten", "fjorten", "femten", "seksten", "sytten", "atten", "nitten"};
    const char* tens[] = {"", "ti", "tyve", "tredive", "fyrre", "halvtreds", "tres", "halvfjerds", "firs", "halvfems"};
    const char* hundreds[] = {"", "hundrede", "to hundrede", "tre hundrede", "fire hundrede", "fem hundrede", "seks hundrede", "syv hundrede", "otte hundrede", "ni hundrede"};
    const char* thousands[] = {"", "tusind", "to tusind", "tre tusind", "fire tusind", "fem tusind", "seks tusind", "syv tusind", "otte tusind", "ni tusind"};

    // Handle numbers between 1 and 9
    if (num < 10) {
        strcat(buffer, ones[num]);
    }
    // Handle numbers between 10 and 99
    else if (num < 100) {
        if (num < 20) {
            strcat(buffer, teens[num - 10]);
        } else {
            int ones_digit = num % 10;
            int tens_digit = num / 10;

            if (ones_digit > 0) {
                strcat(buffer, ones[ones_digit]);
                strcat(buffer, "og");
            }

            strcat(buffer, tens[tens_digit]);
        }
    }
    // Handle numbers between 100 and 999
    else if (num < 1000) {
        int hundreds_digit = num / 100;
        if (hundreds_digit == 1 && !config.skip_one) {
            strcat(buffer, "et hundrede");
        } else {
            strcat(buffer, hundreds[hundreds_digit]);
        }
        num %= 100;

        // Insert "og" only if there's tens or ones to process, and based on the config.use_og flag
        if (num > 0) {
            if (config.use_og) {
                strcat(buffer, " og ");
            } else {
                strcat(buffer, " ");
            }
            digit_to_danish(num, buffer, config);
        }
    }
    // Handle numbers between 1000 and 9999
    else if (num < 10000) {
        int thousands_digit = num / 1000;
        if (thousands_digit == 1 && !config.skip_one) {
            strcat(buffer, "et tusind");
        } else {
            strcat(buffer, thousands[thousands_digit]);
        }
        num %= 1000;

        // Only add "og" if there are tens or ones to process
        if (num > 99) {
            strcat(buffer, " ");
        } else if (num > 0 && config.use_og) {
            strcat(buffer, " og ");
        }

        digit_to_danish(num, buffer, config);
    }
}

void dansketal(int num, char* buffer, struct digits_config config) {
    memset(buffer, 0, 256);  // Clear the buffer

    // Handle numbers too large
    if (num >= 10000 || num <= -10000) {
        strcat(buffer, "Error: Number too large");
        return;
    }

    // Handle negative numbers
    if (num < 0) {
        strcat(buffer, "minus ");
        num = -num;
    }

    // Handle zero
    if (num == 0) {
        strcat(buffer, "nul");
        return;
    }

    // Handle positive numbers
    digit_to_danish(num, buffer, config);

    buffer[255] = '\0';  // Ensure proper string termination by null-terminating the buffer
}

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

int main() {
    struct digits_config config;

    // Test case 1: Basic number without special config
    config = (struct digits_config){1, 1, 0};
    run_test_case(125, config, "et hundrede og femogtyve");

    // Test case 2: Number with skip_one enabled
    config = (struct digits_config){1, 1, 1};
    run_test_case(1001, config, "tusind og et");

    // Test case 3: Number with use_plural disabled
    config = (struct digits_config){0, 1, 0};
    run_test_case(200, config, "to hundrede");

    // Test case 4: Number with use_og disabled
    config = (struct digits_config){1, 0, 0};
    run_test_case(125, config, "et hundrede femogtyve");

    // Test case 5: Negative number
    config = (struct digits_config){1, 1, 0};
    run_test_case(-125, config, "minus et hundrede og femogtyve");

    // Test case 6: Zero
    config = (struct digits_config){1, 1, 0};
    run_test_case(0, config, "nul");

    // Test case 7: Large number
    config = (struct digits_config){1, 1, 0};
    run_test_case(9999, config, "ni tusind ni hundrede og nioghalvfems");

    // Test case 8: Too large number
    config = (struct digits_config){1, 1, 0};
    run_test_case(10000, config, "Error: Number too large");

    // Test case 9: Number under 10
    config = (struct digits_config){1, 1, 0};
    run_test_case(7, config, "syv");

    // Test case 10: Number in the teens
    config = (struct digits_config){1, 1, 0};
    run_test_case(14, config, "fjorten");

    // Test case 11: Number in the twenties (non-plural)
    config = (struct digits_config){0, 1, 0};
    run_test_case(23, config, "treogtyve");

    // Test case 12: Number in the twenties (plural)
    config = (struct digits_config){1, 1, 0};
    run_test_case(23, config, "treogtyve");

    // Test case 13: Number in the thirties
    config = (struct digits_config){1, 1, 0};
    run_test_case(39, config, "niogtredive");

    // Test case 14: Number in the hundreds
    config = (struct digits_config){1, 1, 0};
    run_test_case(100, config, "et hundrede");

    // Test case 15: Number in the thousands
    config = (struct digits_config){1, 1, 0};
    run_test_case(1000, config, "et tusind");

    return 0;
}
