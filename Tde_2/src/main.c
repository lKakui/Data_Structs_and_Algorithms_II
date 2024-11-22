#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/reader.h"

// 1 - montar um índice com uma b++ de chave primária

int main (void){
    FILE *input = fopen("../input/...","RB+");
    if (input == NULL){
        perror("Erro ao abrir o arquivo");
    }

    // read_file(input, sizeof(PRODUCT));

    void *buffer = malloc(sizeof(PRODUCT));

    fread(buffer, sizeof(PRODUCT), 1, input);

    PRODUCT *aux = (PRODUCT *)buffer;

    printf("ID: %d \n", aux->id_product);
    printf("Price: %f \n", aux->price);
    printf("Id Categoria: %lln \n", aux->category_id);
    printf("Code: %s", aux->category_desc);
    printf("Brand: %s", aux->brand);

    
    return 0;
}