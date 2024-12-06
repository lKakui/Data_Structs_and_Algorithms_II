#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/reader.h"
#include "../include/hash.h"

void calc_time(time_t start, time_t end){
    double elapsed_ms = (double)(end - start) * 1000.0 / CLOCKS_PER_SEC;
    printf("Tempo decorrido: %.2f ms\n", elapsed_ms);
}

//1 - montar um índice com uma b++ de chave primária

int main (void){
    FILE *input = fopen("../input/produtos_final_novo.bin","rb");
    if (input == NULL){
        perror("Erro ao abrir o arquivo");
    }

    clock_t start_time = clock();

    if (!read_file(input, sizeof(PRODUCT))){
     printf("deu errado.");
    }

    clock_t end_time = clock();
    calc_time(start_time, end_time);

    // clock_t start_time = clock();

    // if (create_hash_index(input, sizeof(PRODUCT))){
    //     printf("deu certo.");
    // }

    // clock_t end_time = clock();
    // calc_time(start_time, end_time);

    fclose(input);

    return 0;
}