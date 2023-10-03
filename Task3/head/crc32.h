#include <stdlib.h>
#include <stdint.h>

uint32_t crc32c(uint32_t crc, uint8_t byte);
void make_crc_table(unsigned long crcTable[]);
