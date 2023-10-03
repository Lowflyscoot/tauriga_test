#pragma once

#include <stdio.h>
#include "dynamic_array.h"

typedef enum {
    INCORRECT_LENGTH = 1,
    INCORRECT_INPUT = 2,
    MEMORY_ALLOCATION_ERROR = 3,
    OUT_OF_MEMORY = 4,
} errors_t;

void print_errors (d_array_t *array_errors, FILE *target_file);