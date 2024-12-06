#ifndef HASH_H
#define HASH_H

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h> // Para garantir acesso a NULL

#include "../include/reader.h"

#define B 256  // Número de buckets hash

// Estrutura para representar uma hash
typedef struct hash {
    int address;
    float value;
    int key; // Chave para ordenação
    struct hash *next; // Ponteiro para lidar com colisões (encadeamento)
} HASH;

int create_hash_index(FILE *input, size_t model);

#endif // HASH.H
