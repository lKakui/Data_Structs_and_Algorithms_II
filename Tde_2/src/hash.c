#include <stdlib.h>
#include <string.h>

#include "../include/hash.h"

// fazer por marca somar caracteres e calcular via hash

// Função para criar um nó da tabela hash
HASH* create_hash_node(float value, int adress, int sequence) {
    HASH *new = (HASH *)malloc(sizeof(HASH));
    if (new == NULL) {
        perror("Erro de alocação");
        return NULL; // Retorna NULL em caso de erro
    }

    new->adress = adress;
    new->value = value;
    new->key = sequence;
    new->next = NULL;

    return new;
}


// Função para inserir um produto na tabela hash
int insert_hash(HASH **index, PRODUCT *read, int position) {

    int line = (int)(read->value * 100); // Multiplica por 100 para maior granularidade

    // Calcula a posição na tabela hash e a sequência
    int hash_position = line % B; // Índice na tabela hash
    int sequence = line / B;      // Sequência para a chave

    // Cria o nó da hash
    HASH *new_node = create_hash_node(read->value, position, sequence);
    if (new_node == NULL) {
        return -1; // Retorna erro se a alocação falhar
    }

    // Insere o nó na tabela hash usando encadeamento
    if (index[hash_position] == NULL) {
        index[hash_position] = new_node; // Caso não exista colisão
    } else {
        // Insere no início da lista encadeada para lidar com colisões
        new_node->next = index[hash_position];
        index[hash_position] = new_node;
    }

    return 0; // Sucesso
}

HASH ** inicialize_index(){
    HASH **aux = (HASH **)malloc(B * sizeof(HASH*));

    if (aux == NULL) {
        perror("Erro ao alocar memória para a tabela hash");
    }

    for(int i = 0; i < B; i++){
        aux[i] = NULL;
    }

    return aux;
}


int create_hash_index(FILE *input, size_t model){    
    HASH **index = inicialize_index();
    int position = 0;
    void *buffer = malloc(model);

    while ((fread(buffer, model, 1, input) == 1) && (position < 5)){

        PRODUCT *read = (PRODUCT*)buffer;

        insert_hash(index , read, position);

        position++;

        fseek(input,-4,SEEK_CUR);
    }

    return 1;
}