#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/hash.h"

// Function to create a hash node
HASH* create_hash_node(float value, int address, int sequence) {
    HASH *new_node = (HASH *)malloc(sizeof(HASH));
    if (new_node == NULL) {
        perror("Memory allocation error");
        return NULL; // Return NULL in case of an error
    }

    new_node->address = address;
    new_node->value = value;
    new_node->key = sequence;
    new_node->next = NULL;

    return new_node;
}

// Function to insert a product into the hash table
int insert_hash(HASH **index, PRODUCT *read, int position) {
    int line = (int)(read->value * 100); // Multiplica por 100 para maior granularidade

    // Calcula a posição no hash e a sequência
    int hash_position = line % B; // Índice na tabela hash
    int sequence = line / B;      // Sequência para a chave

    // Cria o nó da hash
    HASH *new_node = create_hash_node(read->value, position, sequence);
    if (new_node == NULL) {
        return -1; // Retorna erro se a alocação falhar
    }

    // Caso não haja colisões, insere diretamente
    if (index[hash_position] == NULL) {
        index[hash_position] = new_node; // Sem colisão
    } else {
        // Insere de maneira ordenada
        HASH *current = index[hash_position];
        HASH *previous = NULL;

        // Percorre a lista até encontrar a posição correta
        while (current != NULL && current->key < sequence) {
            previous = current;
            current = current->next;
        }

        // Insere no início da lista se a sequência for menor que o primeiro elemento
        if (previous == NULL) {
            new_node->next = index[hash_position];
            index[hash_position] = new_node;
        } else {
            // Insere no meio ou no final da lista
            previous->next = new_node;
            new_node->next = current;
        }
    }

    return 0; // Sucesso
}


// Function to initialize the hash table
HASH **initialize_index() {
    HASH **aux = (HASH **)malloc(B * sizeof(HASH*));
    if (aux == NULL) {
        perror("Memory allocation error for hash table");
        return NULL;
    }

    for (int i = 0; i < B; i++) {
        aux[i] = NULL;
    }

    return aux;
}

// Function to search in the hash table (implementing a basic search)
HASH* hash_search(HASH **index, float value) {
    int line = (int)(value * 100); 
    int hash_position = line % B;

    HASH *current = index[hash_position];
    while (current != NULL) {
        if (current->value == value) {
            return current; // Found
        }
        current = current->next;
    }
    return NULL; // Not found
}

// Function to print a hash table chain
void print_line(HASH *index, int flag) {
    while (index != NULL) {
        printf("Value: %.2f\n", index->value);
        printf("Address: %d\n", index->address);
        printf("Key: %d\n", index->key);
        printf("------------------------\n");
        if(flag){
            break;
        }
        index = index->next;
    }
}

// Function to create a hash table index from a file
int create_hash_index(FILE *input, size_t model) {
    HASH **index = initialize_index();
    if (index == NULL) {
        return -1; // Memory allocation error
    }

    int position = 0;
    void *buffer = malloc(model);
    if (buffer == NULL) {
        perror("Memory allocation error for buffer");
        free(index);
        return -1;
    }

    while ((fread(buffer, model, 1, input) == 1)) {
        PRODUCT *read = (PRODUCT *)buffer;

        if (insert_hash(index, read, position) != 0) {
            perror("Error inserting into hash table");
            free(buffer);
            free(index);
            return -1;
        }

        position++;
        fseek(input,-4,SEEK_CUR);
    }

    HASH* aux = hash_search(index, 1845.75);

    print_line(aux, 1);

    // Cleanup
    free(buffer);
    for (int i = 0; i < B; i++) {
        HASH *current = index[i];
        while (current != NULL) {
            HASH *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(index);

    return 0;
}