// #define DEBUG

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "hash.h"
#include "bloom_filter.h"

#ifdef DEBUG
    #define DEBUG_PRINT(fmt, args...) printf(fmt, ##args)
#else
    #define DEBUG_PRINT(fmt, args...)
#endif

/* Per gestire il bloom filter viene creato un array di uint8_t di 1024 bit (128 byte, ovvero elementi)
 * Per accedere al byte corretto divido l'indice generato dall'Hash per 8
 * Successivamente creo una "mask" per accedere al bit corretto dentro il byte selezionato
 * Questo viene fatto calcolando index % 8, ovvero il resto di index / 8, e shiftando 1 tante posizioni quanto questo numero calcolato
 * Ad esempio:
 * index      = 13
 * byte_index = 13 / 8 = 1.6 ~= 1
 * bit_mask   = 13 % 8 = 5 -> 00000001 << 5 = 00100000
 * 
 * Pertanto l'esecuzione arriva a questo punto:
 * bloom[1] |= 00100000 (32 in decimale)
 *
 * |= corrisponde ad una not, pertanto imposta ad 1 solo il bit di bit_mask
 * Otteniamo quindi:
 * OR
 * 00100000
 * 00000000
 * --------
 * 00100000
 *
 * Quindi il byte 0 dell'array bloom sarà 00100000
 * */
void bloom_set_bit(uint8_t *bloom, size_t index, size_t num_bits) {
    if (bloom == NULL) {
        printf("Errore: bloom filter è NULL\n");
        return;
    }
    if (index >= num_bits) {
        printf("Errore: indice fuori range\n");
        return;
    }

    size_t byte_index = index / 8;
    uint8_t bit_mask = (1 << (index % 8));

    bloom[byte_index] |= bit_mask;
}

/* Fino al calcolo di bit_value si applica la logica spiegata nel commento di bloom_set_bit()
 * Per trovare il valore di un bit verifico in quali posizioni è presente 1 sia in bloom[byte_index] che nella mask
 *
 * Seguendo l'esempio di prima
 * index      = 13
 * byte_index = 13 / 8 = 1.6 ~= 1
 * bit_mask   = 13 % 8 = 5 -> 00000001 << 5 = 00100000
 * 
 * bloom[1] |= 00100000 (32 in decimale)
 *
 * Arrivati a questo punto viene fatta una AND tra il byte e la mask
 * Se il risultato è 1 il bit è acceso, altrimento è spento
 * Pertanto il valore di ritorno è uguale al valore del bit
 ** */
int bloom_get_bit(uint8_t *bloom, size_t index, size_t num_bits) {
    if (bloom == NULL) {
        printf("Errore: bloom filter è NULL\n");
        return -1;
    }
    if (index >= num_bits) {
        printf("Errore: indice fuori range\n");
        return -1;
    }

    size_t byte_index = index / 8;
    uint8_t bit_mask = (1 << (index % 8));
    int bit_value = (bloom[byte_index] & bit_mask) != 0;

    return bit_value;
}

void bloom_insert_string(uint8_t *bloom, char *str, int num_bits) {
    uint32_t h1 = hash1(str) % num_bits;
    uint32_t h2 = hash2(str) % num_bits;
    uint32_t h3 = hash3(str) % num_bits;

    DEBUG_PRINT("Parola: %s\n", str);
    DEBUG_PRINT("Hash 1: %d\n", h1);
    DEBUG_PRINT("Hash 2: %d\n", h2);
    DEBUG_PRINT("Hash 3: %d\n", h3);

    bloom_set_bit(bloom, h1, num_bits);
    bloom_set_bit(bloom, h2, num_bits);
    bloom_set_bit(bloom, h3, num_bits);
}

int bloom_check_string(uint8_t *bloom, char *str, int num_bits) {
    uint32_t h1 = hash1(str) % num_bits;
    uint32_t h2 = hash2(str) % num_bits;
    uint32_t h3 = hash3(str) % num_bits;

    DEBUG_PRINT("Parola: %s\n", str);
    DEBUG_PRINT("Hash 1: %d\n", h1);
    DEBUG_PRINT("Hash 2: %d\n", h2);
    DEBUG_PRINT("Hash 3: %d\n", h3);
 
    int b1 = bloom_get_bit(bloom, h1, num_bits);
    int b2 = bloom_get_bit(bloom, h2, num_bits);
    int b3 = bloom_get_bit(bloom, h3, num_bits);

    DEBUG_PRINT("Bit Hash 1: %d\n", b1);
    DEBUG_PRINT("Bit Hash 2: %d\n", b2);
    DEBUG_PRINT("Bit Hash 3: %d\n", b3);
 
    return b1 && b2 && b3; 
}

