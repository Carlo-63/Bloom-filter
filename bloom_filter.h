#include <stdint.h>
#include <stdlib.h>

#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

void bloom_set_bit(uint8_t *bloom, size_t index, size_t num_bits);
int bloom_get_bit(uint8_t *bloom, size_t index, size_t num_bits);
void bloom_insert_string(uint8_t *bloom, char *str, int num_bits);
int bloom_check_string(uint8_t *bloom, char *str, int num_bits);

#endif
