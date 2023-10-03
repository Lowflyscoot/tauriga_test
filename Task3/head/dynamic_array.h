#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    uint8_t *array;
    uint8_t *array_pointer;
    size_t size;
    size_t used_memory;
} d_array_t;

d_array_t *init_array (size_t size);
void add_element (d_array_t *array, uint8_t byte);
size_t get_anchor_position(d_array_t *array, char *anchor, size_t start_pos);
size_t count_anchors(d_array_t *array, char *anchor);
void reset_pointer(d_array_t *array);
uint8_t get_array_element_with_shift(d_array_t *array);
void set_pointer_by_position(d_array_t *array, size_t position);