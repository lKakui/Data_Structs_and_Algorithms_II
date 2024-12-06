#ifndef READER_H
#define READER_H

#include <stdio.h>
#include <stdbool.h>
#include <stddef.h> // Para garantir acesso a NULL

#define N 5 // Número máximo de filhos de um nó da B-tree

// Estrutura para representar um nó de uma B-tree
typedef struct BTreeNode {
    int num_keys;                     // Número atual de chaves no nó
    long long int keys[N - 1];        // Array de chaves (máximo de N-1 chaves)
    int address[N - 1];               // Array de endereços associados às chaves
    struct BTreeNode *children[N];    // Ponteiros para os filhos
    int is_leaf;                     // Indica se o nó é uma folha (true/false)
} BTREE;

// Estrutura para armazenar informações sobre um produto
typedef struct Product {
    long long int product_id;         // ID do produto
    char brand[16];                   // Marca do produto (máx. 15 caracteres + '\0')
    float value;                      // Valor do produto
} PRODUCT;

int read_file(FILE *input, size_t model);

#endif // READER_H
