#include "error_operations.h"
#include "dynamic_array.h"
#include <stdio.h>

void print_errors (d_array_t *array_errors, FILE *target_file)
{
    for (int i = 0; i < array_errors->used_memory; i++)
    reset_pointer(array_errors);
    switch (get_array_element_with_shift(array_errors, array_errors))
    {
        case INCORRECT_LENGTH:
            fprintf(target_file, "<ERROR: Incorrect length of input>\n\n");
            break;
        case INCORRECT_INPUT:
            fprintf(target_file, "<ERROR: Incorrect input>\n\n");
            break;
        case MEMORY_ALLOCATION_ERROR:
            fprintf(target_file, "<ERROR: Failed to allocate memory>\n\n");
            break;
        case OUT_OF_MEMORY:
            fprintf(target_file, "<ERROR: Trying work with elements in out of allocated memory>\n\n");
            break;
        default:
            break;
    }
}