#pragma once

#include <stdint.h>

typedef struct {
    uint8_t type;
    size_t length;
    size_t final_length;
    uint8_t* data;
    uint32_t crc32;
} message_t;

uint32_t mask_parsing (d_array_t *item, size_t position, d_array_t *error_array);
uint8_t pack_byte (d_array_t *item, d_array_t *error_array);
message_t *message_parsing (d_array_t *item, size_t position, d_array_t *error_array);
