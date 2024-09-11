#ifndef DIGITS_CONFIG_H
#define DIGITS_CONFIG_H

#include <stdbool.h>

// Struct for configuration options
struct digits_config {
    bool use_plural;  // If true, use plural forms where applicable
    bool use_og;      // If true, include "og" between tens and ones
    bool skip_one;    // If true, skip "et" in front of hundreds or thousands
};

#endif  // DIGITS_CONFIG_H