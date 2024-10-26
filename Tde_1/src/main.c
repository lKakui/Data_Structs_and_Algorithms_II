#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../include/reader.h"
#include "../include/fragmentation.h"
#include "../include/merge.h"
#include "../include/index.h"

int compare_user(const void *arg1, const void *arg2){
    return ((UserEntry *)arg1)->user_session - ((UserEntry *)arg2)->user_session;
}

int compare_prod(const void *arg1, const void *arg2)
{
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

    int n_fragments;

    // inicio = clock();
    // FILE *data = fopen("../input/2019-Nov.csv","r");
    // file_reader (data);
    // end = clock();
    // print_time(inicio, end);
    
    // // prodfile ---------------------------------------------------------------
    // inicio = clock();
    // FILE *prod_data = fopen("../output/prodfile.bin","rb+");
    // if(prod_data == NULL){
    //     perror("erro ao abrir o arquivo.");
    // }
    // n_fragments = file_sort_frag (prod_data, sizeof(ProductEntry),compare_prod);
    // end = clock();
    // print_time(inicio, end);

    // inicio = clock();
    // fileprod_merge(n_fragments);
    // fclose(prod_data);
    // end = clock();
    // print_time(inicio, end);

    //serfile ---------------------------------------------------------------
    // inicio = clock();
    // FILE *user_data = fopen("../output/userfile.bin","rb+");
    // if(user_data == NULL){
    //     perror("erro ao abrir o arquivo.");
    // }

    // n_fragments = file_sort_frag (user_data, sizeof(UserEntry),compare_user);
    // end = clock();
    // print_time(inicio, end);

    //  inicio = clock();
    //  fileuser_merge(n_fragments);
    //  fclose(user_data);
    //  end = clock();
    //  print_time(inicio, end);

    // // user - index_part ---------------------------------------------------------------
    // FILE *user_data = fopen("../output/userfilesort.bin","rb+");
    // if(prod_data == NULL){
    //     perror("erro ao abrir o arquivo.");
    // }
    // create_partial_index(user_data, sizeof(UserEntry),"user");

    //product - index_part ------------------------------------------------------------
    // FILE *prod_data = fopen("../output/fileprodsort.bin","rb+");
    // if(prod_data == NULL){
    //     perror("erro ao abrir o arquivo.");
    // }
    // create_partial_index(prod_data, sizeof(ProductEntry),0);
    // fclose(prod_data); 

    //ind - bin --------------------------------------------------------------------------
    FILE *prod_data9 = fopen("../output/fileprodsort.bin","rb");
    if(prod_data9 == NULL){
        perror("nao abriu arq 1");
    }

    void *prod_buffer = malloc(sizeof(ProductEntry));

    for (int j = 0; j < 20; j++){
        fread(prod_buffer, sizeof(ProductEntry),1,prod_data9);
        printf("Id: %d\n", ((ProductEntry *)prod_buffer)->product_id);
        printf("Price: %f\n", ((ProductEntry *)prod_buffer)->price);
        printf("Category: %llu\n", ((ProductEntry *)prod_buffer)->category_id);
        printf("Category_code: %s\n", ((ProductEntry *)prod_buffer)->category_code);
        printf("Brand: %s\n\n", ((ProductEntry *)prod_buffer)->brand);
        printf("-----------------------------------------\n");
    }
    printf("\n\n\n");
    rewind(prod_data9);

    
    FILE *index_data = fopen("../output/partprod_index.bin", "rb");
    if(index_data == NULL){
        perror("nao abriu arq 1");
    }

    void *buffer = malloc(sizeof(PART_INDEX));

    for(int i = 0; i < 40; i++){
        fread(buffer, sizeof(PART_INDEX),1, index_data);
        printf("Key: %d / Posic: %d \n", ((PART_INDEX *)buffer)->key, ((PART_INDEX *)buffer)->end);
    }
    rewind(index_data);
    printf("nao comecou busca binaria");
    int a = bin_search_index(index_data, prod_data9, 1005099);
    printf("\n%d \n\n", a); 
    if (a == -1){
        printf("produto não encontrado");
    }
    else {
        printf("começou a achar o index");
        int x = find_in_index(prod_data9, sizeof(ProductEntry), 1005099);    
    }
}