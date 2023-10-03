#include "dynamic_array.h"

// Checking violation of borders of a dynamic array
d_array_t *check_length (d_array_t *item)
{
    // Trying to get element out of borders of array
    if (item->array_pointer > (item->array + item->size))
    {
        reset_pointer(item);
    }
    // Allocated memory is end, need to allocate new memory field with more size
    if (item->used_memory >= item->size)
    {
        // Calc shift between starting pointer and floating pointer to save it
        size_t pointer_shift = item->array_pointer - item->array;
        // Memory for temporary storage
        uint8_t *temporary_array = malloc(item->size);
        // Saving old data
        memcpy(temporary_array, item->array, item->size);
        // Freeing old smal memory field
        free (item->array);

        // New more sized memory field allocation
        item->array = malloc(item->size + 10);
        // Loading saved data to mew memory
        memcpy(item->array, temporary_array, item->size);
        // Increase size field
        item->size += 10;
        // Restore of floating pointer
        item->array_pointer = item->array + pointer_shift;
        // Freeing of temporary memory
        free(temporary_array);
    }
    return item;
}

// Init the new dynamic array
d_array_t *init_array (size_t size)
{
    d_array_t *dynamic_array = malloc(sizeof(d_array_t));
    dynamic_array->array = malloc(size);
    dynamic_array->array_pointer = dynamic_array->array;
    dynamic_array->size = size;
    dynamic_array->used_memory = 0;
}

// Safe adding element to dynamic array
void add_element (d_array_t *array, uint8_t byte)
{
    // Check problems
    array = check_length(array);
    // Add new element to the end of array
    *array->array_pointer = byte;
    array->array_pointer++;
    array->used_memory++;
}

// Reset floating pointer to the starting address of array
void reset_pointer(d_array_t *array)
{
    array->array_pointer = array->array;
}

// Set arbitary position of floating pointer
void set_pointer_by_position(d_array_t *array, size_t position)
{
    array->array_pointer = array->array + position;
    array = check_length(array);
}

// Get nest element and shift pointer to next
uint8_t get_array_element_with_shift(d_array_t *array)
{
    uint8_t result = *array->array_pointer++;
    array = check_length(array);
    return result;
}

// Finding out some string in a dynamic array and return position after it 
size_t get_anchor_position(d_array_t *array, char *anchor, size_t start_pos)
{
    int anchor_len = strlen(anchor) + 1;
    char *found_part = malloc(anchor_len);
    uint8_t *pointer = array->array;
    size_t result = 0;

    // Go thought all elements and finding out target string
    for (int i = start_pos; i < (array->size - anchor_len); i++)
    {
        memcpy(found_part, &pointer[i], anchor_len);
        found_part[anchor_len - 1] = '\0';

        if (strcmp(found_part, anchor) == 0)
        {
            result = i + anchor_len - 1;
            break;
        }
    }

    free(found_part);
    return result;
}

// Counting all string occurances in array
size_t count_anchors(d_array_t *array, char *anchor)
{
    int anchor_len = strlen(anchor) + 1;
    char *found_part = malloc(anchor_len);
    uint8_t *pointer = array->array;
    size_t result = 0;

    // Go thought all elements and finding out target string but not breaking but counting elements 
    for (int i = 0; i < (array->size - anchor_len); i++)
    {
        memcpy(found_part, &pointer[i], anchor_len);
        found_part[anchor_len - 1] = '\0';

        if (strcmp(found_part, anchor) == 0)
        {
            result++;
        }
    }

    free(found_part);
    return result;
}