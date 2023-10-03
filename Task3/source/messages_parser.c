#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "crc32.h"
#include "singly_linked_list.h"
#include "dynamic_array.h"

#define MIN(x,y) (x < y ? x:y)
#define MAX(x,y) (x > y ? x:y)

typedef struct {
    uint8_t type;
    size_t length;
    size_t final_length;
    uint8_t* data;
    uint32_t crc32;
} message_t;


uint32_t mask_parsing (d_array_t *item, size_t position);
uint8_t pack_byte (d_array_t *item);
message_t *message_parsing (d_array_t *item, size_t position);
uint32_t mask_parsing (d_array_t *item, size_t position);
uint32_t mask_parsing (d_array_t *item, size_t position);


// Convert symbol from input to uint-number
uint8_t ascii_to_half_byte(const uint8_t symbol_code)
{
    if (symbol_code >= 0x41 && symbol_code <= 0x46)
    {
        return (symbol_code - 0x41 + 0x0A);
    }
    else if (symbol_code >= 0x30 && symbol_code <= 0x39)
    {
        return (symbol_code - 0x30);
    }
}


// Packing two following symbols to one byte
uint8_t pack_byte (d_array_t *item)
{
    uint8_t result_byte = 0;
    result_byte = ascii_to_half_byte(get_array_element_with_shift(item)) << 4;
    result_byte |= ascii_to_half_byte(get_array_element_with_shift(item));
    return result_byte;
}


// Parsing of input array to message struct
message_t *message_parsing (d_array_t *item, size_t position)
{
    message_t *current_message = malloc(sizeof(message_t));

    // Set position of next element
    set_pointer_by_position(item, position);

    // Get the type
    current_message->type = pack_byte(item);
    // Get the length
    current_message->length = pack_byte(item);

    // Count length, myltiplyed by 4
    current_message->final_length = current_message->length;
    if (current_message->final_length % 4)
    {
        current_message->final_length += 4 - (current_message->final_length % 4);
    }

    // Allocation memory for message and creating floating pointer data_pointer to movement of data bytes
    uint32_t *data_pointer = malloc(sizeof(int*) * current_message->length);
    uint32_t *data_start = data_pointer;
    uint32_t tmp;
    
    // Getting data from array
    for (int i = 0; i < (current_message->final_length / 4); i++)
    {
        *data_pointer = 0;
        for (int j = 0; j < 4; j++)
        {
            if (((i * 4) + j) >= current_message->length) break;
            *data_pointer |= (uint32_t)pack_byte(item) << (24 - j * 8);
        }
        data_pointer++;
    }

    // Saving data pointer to struct
    current_message->data = (uint8_t*)data_start;    

    // Save CRC32
    current_message->crc32 = 0;
    for (int i = 0; i < 4; i++)
    {
        current_message->crc32 |= (pack_byte(item) << (24 - i * 8));
    }

    return current_message;
}


// Parsing of mask and saving it to uint32
uint32_t mask_parsing (d_array_t *item, size_t position)
{
    // Floating pointer to position in array
    set_pointer_by_position(item, position);

    uint32_t result_mask = 0;
    uint8_t byte = 0;

    // Saving mask
    for (int i = 0; i < 4; i++)
    {
        byte = pack_byte(item);
        result_mask |= (byte << (24 - i * 8));
    }

    return result_mask;
}


// Overlay mask on data array and creating new changed message struct
message_t *mask_overlay (message_t *recieved_message, uint32_t mask)
{
    // Memory for new message
    message_t *new_message = malloc(sizeof(message_t));
    // Allocation of memory field for edit data
    uint32_t *process_data = malloc(sizeof(uint32_t) * recieved_message->final_length);
    uint32_t *process_data_pointer = process_data;
    // Save old data field
    memcpy(process_data, recieved_message->data, recieved_message->final_length);
    // Allocated memory for eddited message struct and init fields beside of data
    new_message->type = recieved_message->type;
    new_message->final_length = recieved_message->final_length;
    new_message->length = recieved_message->length;
    new_message->data = malloc(sizeof(uint32_t) * recieved_message->final_length);

    // Overlaying mask to all data fields
    uint32_t temp;
    for (int i = 0; i < (recieved_message->final_length); i++)
    {
        temp = *process_data_pointer & mask;
        *process_data_pointer = *process_data_pointer & mask;
        process_data_pointer++;
    }

    // Calc CRC32 of message without crc
    uint32_t CRC32 = 0xFFFFFFFFull;
    CRC32 = crc32c(CRC32, new_message->type);
    CRC32 = crc32c(CRC32, new_message->final_length);

    uint8_t byte_block[4] = {};
    for (int i = 0; i < (recieved_message->final_length / 4); i++)
    {
        memcpy(byte_block, &process_data[i], 4);
        for (int j = 0; j < 4; j++)
        {
            CRC32 = crc32c(CRC32, byte_block[3 - j]);
        }
    }

    // Saving crc32 to struct
    new_message->crc32 = CRC32;

    new_message->data = (uint8_t*)process_data;
    
    return new_message;
}


// Output of old and new verions of a message to a file
void print_message_to_file (message_t *old_message, message_t *new_message, FILE *target_file)
{
    uint32_t *data_pointer = NULL;
    
    fprintf(target_file, "Message type: %d\n", old_message->type);
    fprintf(target_file, "Initial message length: %d\n", old_message->length);
    fprintf(target_file, "Initial message data bytes: ");

    // Print tetrads of initial messate
    data_pointer = (uint32_t*)old_message->data;
    for (int i = 0; i < (old_message->length / 4); i++)
    {
        fprintf(target_file, "%08X ", *data_pointer);
        data_pointer++;
    }

    // Print incomplete tetrad one byte at time
    int remaining_bytes = old_message->length % 4;
    // Needed turn around bytes inside the uint32
    uint8_t remaining_part [4] = {((uint8_t*)data_pointer)[3], ((uint8_t*)data_pointer)[2], ((uint8_t*)data_pointer)[1], ((uint8_t*)data_pointer)[0]};
    // Print by byte
    if (remaining_bytes > 0) {
        uint8_t *byte_pointer = (uint8_t*)data_pointer;
        for (int i = 0; i < remaining_bytes; i++) {
            fprintf(target_file, "%02X", remaining_part[i]);
            byte_pointer++;
        }
    }

    fprintf(target_file, "\nInitial CRC-32: ");
    fprintf(target_file, "0x%08x\n", old_message->crc32);

    fprintf(target_file, "Modified message length: %d\n", new_message->final_length);
    fprintf(target_file, "Modified message data bytes with mask: ");

    data_pointer = (uint32_t*)new_message->data;
    for (int i = 0; i < (new_message->final_length / 4); i++)
    {
        fprintf(target_file, "%08X ", *data_pointer);
        data_pointer++;
    }

    fprintf(target_file, "\nModified CRC-32: ");
    fprintf(target_file, "0x%08x\n\n", new_message->crc32);
}



int main (void)
{
    // Opening of file to read
    FILE *input_file;
    input_file = fopen("input.txt", "r");
    // Opening of file to write
    FILE *output_file;
    output_file = fopen("output.txt", "a");

    // Storage of each symbol from an input
    char symbol;
    
    // Packing all input to a dynamic array
    d_array_t *d_array_input = init_array(10);
    do {
        symbol = fgetc(input_file);
        add_element(d_array_input, symbol);
    } while (symbol != EOF);
    
    // The number of pairs is equal to the minimum number of messages and masks.
    size_t pair_number = 0;
    pair_number = MIN(count_anchors(d_array_input, "mess="), count_anchors(d_array_input, "mask="));

    // Storage for mask
    uint32_t mask = 0;

    // Positions of nexst message and mask to easy movement without them in the array
    size_t array_mess_position = 0;
    size_t array_mask_position = 0;
    // Structures for storaging the next message in initial and modified version
    message_t *old_mess = NULL;
    message_t *new_mess = NULL;
    // Movement for each pair of message and mask
    for (int i = 0; i < pair_number; i++)
    {
        // Find out the position of the next message
        array_mess_position = get_anchor_position(d_array_input, "mess=", array_mess_position);
        // For the each anchor "mess=" performing parsing
        old_mess = message_parsing(d_array_input, array_mess_position);
        // Find out the position of the next mask
        array_mask_position = get_anchor_position(d_array_input, "mask=", array_mask_position);
        // For the each anchor "mask=" performing parsing
        mask = mask_parsing(d_array_input, array_mask_position);
        // Overlaying mask to message data and recalculation of a CRC-32
        new_mess = mask_overlay(old_mess, mask);
        // Output to the file
        print_message_to_file(old_mess, new_mess, output_file);
        // Freeing of memory
        free(old_mess->data);
        free(old_mess);
        free(new_mess->data);
        free(new_mess);
    }

    // Freeing of memory
    free(d_array_input);
}