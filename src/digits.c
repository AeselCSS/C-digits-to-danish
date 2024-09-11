#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "test.h"
#include "digits_config.h"

void digit_to_danish(int num, char* buffer, struct digits_config config) {
    
    // Define the arrays for the different parts of the number
    // ones, teens, tens
    const char* ones[] = {"", "et", "to", "tre", "fire", "fem", "seks", "syv", "otte", "ni"};
    const char* teens[] = {"ti", "elleve", "tolv", "tretten", "fjorten", "femten", "seksten", "sytten", "atten", "nitten"};
    const char* tens[] = {"", "ti", "tyve", "tredive", "fyrre", "halvtreds", "tres", "halvfjerds", "firs", "halvfems"};

    // NOTE: All below cases are handled recursively by calling digit_to_danish with the remaining number and appending the result to the buffer

    // Handle billions
    if (num >= 1000000000) {
        if (num / 1000000000 != 1) {
            digit_to_danish(num / 1000000000, buffer, config);
            strcat(buffer, " ");
        }
        strcat(buffer, (num / 1000000000 == 1) ? "en milliard" : "milliarder");
        num %= 1000000000;

        if (num == 1 && config.skip_one) {
            strcat(buffer, " og et");
            return;
        }

        if (num > 0) strcat(buffer, " ");
    }

    // Handle millions
    if (num >= 1000000) {
        if (num / 1000000 != 1) {
            digit_to_danish(num / 1000000, buffer, config);
            strcat(buffer, " ");
        }
        strcat(buffer, (num / 1000000 == 1) ? "en million" : "millioner");
        num %= 1000000;

        if (num == 1 && config.skip_one) {
            strcat(buffer, " og et");
            return;
        }
        
        if (num > 0) strcat(buffer, " ");
    }

    // Handle thousands
    if (num >= 1000) {
        digit_to_danish(num / 1000, buffer, config);
        strcat(buffer, " ");
        strcat(buffer, (num / 1000 == 1) ? "tusind" : "tusind");
        num %= 1000;

        if (num == 1 && config.skip_one) {
            strcat(buffer, " og et");
            return;
        }
        
        if (num > 0) strcat(buffer, " ");
    }

    // Handle hundreds
    if (num >= 100) {
        digit_to_danish(num / 100, buffer, config);
        strcat(buffer, " ");
        strcat(buffer, "hundrede");
        num %= 100;

        if (num == 1 && config.skip_one) {
            strcat(buffer, " og et");
            return;
        }
        
        if (num > 0 && config.use_og) strcat(buffer, " og ");
        else if (num > 0) strcat(buffer, " ");
    }

    // Handle 20-99
    if (num >= 20) {
        if (num % 10 > 0) {
            strcat(buffer, ones[num % 10]);
            strcat(buffer, "og");
        }
        strcat(buffer, tens[num / 10]);
        return;
    }

    // Handle 10-19
    if (num >= 10 && num < 20) {
        strcat(buffer, teens[num - 10]);
        return;
    }

    // Handle 1-9
    if (num > 0) {
        if (num == 1 && config.skip_one) return;
        strcat(buffer, ones[num]);
    }
}

void dansketal(int num, char* buffer, struct digits_config config) {
    // Clear the buffer before starting
    memset(buffer, 0, 256);  

    // Check if the number is within the valid range
    if (num > INT32_MAX || num < INT32_MIN) {
        strcat(buffer, "Error: Number is not within the valid range");
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

    // Remove leading whitespace if present
    if (buffer[0] == ' ') {
        memmove(buffer, buffer + 1, strlen(buffer));
    }

    // Remove trailing whitespace if present
    if (buffer[strlen(buffer) - 1] == ' ') {
        buffer[strlen(buffer) - 1] = '\0';
    }

    // Add null terminator to the end of the buffer to ensure it is a valid string
    buffer[255] = '\0';
}

int main() {
    struct digits_config config;
    
    // Test case 1: Basic number without special config
    config = (struct digits_config){true, true, false};
    run_test_case(125, config, "et hundrede og femogtyve");

    // Test case 2: Number with skip_one enabled
    config = (struct digits_config){true, true, true};
    run_test_case(1001, config, "tusind og et");

    // Test case 3: Number with use_plural disabled
    config = (struct digits_config){false, true, false};
    run_test_case(200, config, "to hundrede");

    // Test case 4: Number with use_og disabled
    config = (struct digits_config){true, false, false};
    run_test_case(125, config, "et hundrede femogtyve");

    // Test case 5: Negative number
    config = (struct digits_config){true, true, false};
    run_test_case(-125, config, "minus et hundrede og femogtyve");

    // Test case 6: Zero
    config = (struct digits_config){true, true, false};
    run_test_case(0, config, "nul");

    // Test case 7: Large number
    config = (struct digits_config){true, true, false};
    run_test_case(9999, config, "ni tusind ni hundrede og nioghalvfems");

    // Test case 8: Number under 20
    config = (struct digits_config){true, true, false};
    run_test_case(17, config, "sytten");

    // Test case 9: Number under 10
    config = (struct digits_config){true, true, false};
    run_test_case(7, config, "syv");

    // Test case 10: Number in the teens
    config = (struct digits_config){true, true, false};
    run_test_case(14, config, "fjorten");

    // Test case 11: Number in the twenties (non-plural)
    config = (struct digits_config){false, true, false};
    run_test_case(23, config, "treogtyve");

    // Test case 12: Number in the twenties (plural)
    config = (struct digits_config){true, true, false};
    run_test_case(23, config, "treogtyve");

    // Test case 13: Number in the thirties
    config = (struct digits_config){true, true, false};
    run_test_case(39, config, "niogtredive");

    // Test case 14: Number in the hundreds
    config = (struct digits_config){true, true, false};
    run_test_case(100, config, "et hundrede");

    // Test case 15: Number in the thousands
    config = (struct digits_config){true, true, false};
    run_test_case(1000, config, "et tusind");

    // Test case 16: Number in the millions- PASSED
    config = (struct digits_config){true, true, false};
    run_test_case(1000000, config, "en million");

    // Test case 17: Number in the billions
    config = (struct digits_config){true, true, false};
    run_test_case(1000000000, config, "en milliard");

    // Test case 18: INT32_MAX boundry test
    config = (struct digits_config){true, true, false};
    run_test_case(INT32_MAX, config, "to milliarder et hundrede og syvogfyrre millioner fire hundrede og treogfirs tusind seks hundrede og syvogfyrre");

    // Test case 19: INT32_MIN boundry test
    config = (struct digits_config){true, true, false};
    run_test_case((INT32_MIN + 1), config, "minus to milliarder et hundrede og syvogfyrre millioner fire hundrede og treogfirs tusind seks hundrede og syvogfyrre");
    
    //Test case 20: tens of thousands
    config = (struct digits_config){true, true, false};
    run_test_case(20000, config, "tyve tusind");

    //Test case 21: hundreds of thousands
    config = (struct digits_config){true, true, false};
    run_test_case(300000, config, "tre hundrede tusind");

    //Test case 22: tens of thousands extended
    config = (struct digits_config){true, true, false};
    run_test_case(23456, config, "treogtyve tusind fire hundrede og seksoghalvtreds");

    //Test case 23: hundreds of thousands extended
    config = (struct digits_config){true, true, false};
    run_test_case(345678, config, "tre hundrede og femogfyrre tusind seks hundrede og otteoghalvfjerds");
    
    //Test case 24: testing 101 with skip_one enabled
    config = (struct digits_config){true, true, true};
    run_test_case(101, config, "hundrede og et");

    return 0;
}