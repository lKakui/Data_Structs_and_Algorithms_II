#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/reader.h"
#include "../include/hash.h"

// 1 - montar um índice com uma b++ de chave primária

int main (void){
    FILE *input = fopen("../input/produtos_final_novo.bin","rb");
    if (input == NULL){
        perror("Erro ao abrir o arquivo");
    }

//    if (read_file(input, sizeof(PRODUCT))){
//     printf("deu certo.");
//    }

   if (create_hash_index(input, sizeof(PRODUCT))){
    printf("deu certo.");
   }

    fclose(input);

    return 0;
}