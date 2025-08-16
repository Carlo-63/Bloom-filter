#include <stdint.h>
#include <stddef.h>

#include "hash.h"

// Hash 1: variante di djb2
uint32_t hash1(const char *str) {
    uint32_t hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    return hash;
}

// Hash 2: variante di sdbm
uint32_t hash2(const char *str) {
    uint32_t hash = 0;
    int c;
    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;
    return hash;
}

// Hash 3: semplice rotazione XOR
uint32_t hash3(const char *str) {
    uint32_t hash = 0;
    int c;
    while ((c = *str++))
        hash ^= (hash << 5) + (hash >> 2) + c;
    return hash;
}
