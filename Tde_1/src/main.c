#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/reader.h"
#include "../include/fragmentation.h"
#include "../include/merge.h"

int compare_user(const void *arg1, const void *arg2){
    return ((UserEntry *)arg1)->user_id - ((UserEntry *)arg2)->user_id;
}

int compare_prod(const void *arg1, const void *arg2){
    return ((ProductEntry *)arg1)->product_id - ((ProductEntry *)arg2)->product_id;
}

void print_time(clock_t ini, clock_t end){
    double cpu_time_used = ((double) (end - ini)) / CLOCKS_PER_SEC;
    printf("Tempo de execução: %f segundos\n", cpu_time_used);
    return;   
}

int main(){
    clock_t inicio;
    clock_t end;

    inicio = clock();
    FILE *data = fopen("../input/2019-Nov.csv","r");
    file_reader (data);
    end = clock();
    print_time(inicio, end);

    inicio = clock();
    FILE *prod_data = fopen("../output/userfile.bin","rb+");
    if(prod_data == NULL){
        perror("erro ao abrir o arquivo.");
    }
    int n_fragments = file_sort_frag (prod_data, sizeof(ProductEntry),compare_prod);
    end = clock();
    print_time(inicio, end);

    inicio = clock();
    file_merge(n_fragments);
    end = clock();
    print_time(inicio, end);
}