#include <stdio.h>
#include <stdlib.h>

#include "index.h"
#include "reader.h"

int create_partial_index(FILE* input, size_t data_size, int flag){
    void *buffer = malloc(data_size);
    int count = 0;
    int limit = 0;
    PART_INDEX aux;

    FILE *partial_index = fopen("../output/partprod_index.bin", "wb");
    if(partial_index == NULL){
        perror("erro ao abrir o arquivo !!!!.");
    }

    if(flag){
        UserEntry *entry;
        while(fread(buffer, data_size, 1, input) == 1){
            entry = (UserEntry*)buffer;
            if(count == 0){
                limit = entry->user_id + 100;
                aux.key = entry->user_id;
                aux.end = 0;
            }
            else if(limit < entry->user_id){
                fwrite(&aux,sizeof(PART_INDEX),1, partial_index);
                limit = entry->user_id + 100;
                aux.end = count;
            }
            else{
                aux.key = entry->user_id;
            }
            count++;
        }
        if (count > 0 && limit < entry->user_id) {
            aux.key = entry->user_id;
            fwrite(&aux, sizeof(PART_INDEX), 1, partial_index);
            aux.end = count;
        }
    }

    else{
    ProductEntry entry; 
    while (fread(&entry, data_size, 1, input) == 1) {
        if (count == 0) {
            limit = entry.product_id + 100;
            aux.end = 0;
            aux.key = entry.product_id;
        } else if (entry.product_id > limit) {
            fwrite(&aux, sizeof(PART_INDEX), 1, partial_index);
            printf("%d, %d \n", aux.end, aux.key);
            aux.end = count;
            aux.key = entry.product_id;
            limit = aux.key + 100;
        }
        count++;
    }

    if (count > 0) {
        aux.end = count;
        fwrite(&aux, sizeof(PART_INDEX), 1, partial_index);
    }
}


    fclose(partial_index);
    return 1;
}

int bin_search_index(FILE* index, FILE* base , int key){
    void *buffer = malloc(sizeof(PART_INDEX));
    fseek(index,0, SEEK_END);
    int qntd = ftell(index)/sizeof(PART_INDEX);

    int esq = 0, dir= qntd-1;

    while (esq <= dir) {
        int mid = (esq + dir) / 2;

        fseek(index, mid * sizeof(PART_INDEX), SEEK_SET);
        
        PART_INDEX part;
        fread(&part, sizeof(PART_INDEX), 1, index);

        if (key == part.key) {
            fseek(base, part.end * sizeof(ProductEntry), SEEK_SET);
            return 0; // Key found
        }


        if (esq + 1 == dir) {
                PART_INDEX prev_part;
                fseek(index, (mid - 1) * sizeof(PART_INDEX), SEEK_SET);
                fread(&prev_part, sizeof(PART_INDEX), 1, index);
                
                if (key > prev_part.key && key < part.key) {
                    fseek(base, prev_part.end * sizeof(ProductEntry), SEEK_SET);
                    return part.end; // Key is between prev_part and part
            }
            // If we reach here, we break out of the loop since there are no more candidates to check
            break;
        }

        // Adjust the search boundaries
        if (key < part.key) {
            dir = mid;
        } else {
            esq = mid;
        }
    }
    return -1; // Indicate not found
}

int find_in_index(FILE* input, size_t data_size, int key){
    void *buffer = malloc(data_size);

    fread(buffer, data_size, 1, input);

    int flag = 0;

    if(data_size == sizeof(ProductEntry)){
        while(((ProductEntry *)buffer)->product_id <= key){
            if(key == ((ProductEntry*)buffer)->product_id){
                printf("\nID: %d \n",((ProductEntry*)buffer)->product_id);
                printf("Price: %f \n",((ProductEntry*)buffer)->price);
                printf("Category: %s \n",((ProductEntry*)buffer)->category_id);
                printf("Category_code: %s \n",((ProductEntry*)buffer)->category_code);
                printf("Brand: %s \n",((ProductEntry*)buffer)->brand);
                flag = 1;
            }
        }
        if (flag){
            printf("chave %d não está presente.");
        }
        return 1;
    }

    if(data_size == sizeof(UserEntry)){
        while(((UserEntry *)buffer)->user_id <= key){
            if(key == ((UserEntry *)buffer)->user_id){
               printf("User_id: %d \n",((ProductEntry*)buffer)->price);
                printf("Category: %s \n",((ProductEntry*)buffer)->category_id);
                printf("Category_code: %s \n",((ProductEntry*)buffer)->category_code);
                printf("Brand: %s \n",((ProductEntry*)buffer)->brand);
                flag = 1;
            }
        }
        if (flag){
            printf("chave %d não está presente.");
        }
        return 1;
    }


}