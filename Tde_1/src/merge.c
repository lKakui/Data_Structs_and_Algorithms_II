#include <stdio.h>
#include <stdlib.h>

#include "../include/merge.h"

// int a(FILE * last, size_t data_size){
//     fseek(last,0, SEEK_END);
//     long file_size = ftell(last);
//     int file_data = (int *)(file_size/data_size);
//     rewind(last);
//     return file_data;
// }

FILE **open_parts(int fragments) {
    FILE **open_files = malloc(fragments * sizeof(FILE *)); // Corrigido para FILE *
    if (open_files == NULL) {
        perror("Erro na alocação de memória");
        return NULL;
    }
    
    char buffer[35];
    for (int i = 0; i < fragments; i++) { // Mudança no índice
        sprintf(buffer, "../output/partitions/fragment_%d.bin", i + 1);
        printf("Abrindo o arquivo: %s \n", buffer);
        open_files[i] = fopen(buffer, "rb");
        if (open_files[i] == NULL) {
            perror("Erro ao abrir as partições");
            // Libera memória de arquivos abertos antes de retornar
            for (int j = 0; j < i; j++) {
                fclose(open_files[j]);
            }
            free(open_files);
            return NULL; // Retorna NULL em caso de erro
        }
    }
    return open_files;
}

int close_all(FILE **arq, int n_fragments){
    int i;
    for (i=0; i < n_fragments; i++){
        fclose(arq[i]);
    }
    return 1;
}

int find_small(int *vet, int vet_size) {
    int small = -1;
    int posi = -1;

    for (int i = 0; i < vet_size; i++) {
        if (vet[i] != 0) { // Ignora zeros
            if (small == -1 || vet[i] < small) { // Se é o primeiro não zero ou menor que o atual
                small = vet[i];
                posi = i;
            }
        }
    }
    return posi;
}


int fileprod_merge(int n_fragments){
    FILE **all_files = open_parts(n_fragments);
    int *vet = (int*)malloc(sizeof(int) * n_fragments);
    ProductEntry *aux_prod = (ProductEntry*)malloc(sizeof(ProductEntry));
    ProductEntry *index = (ProductEntry*)malloc(sizeof(ProductEntry)*n_fragments);
    
    for(int i = 0; i < n_fragments; i++){ //carrega as partições
        FILE *aux_file = all_files[i];
        if(fread(aux_prod,sizeof(ProductEntry),1,aux_file) != 1){//le todo as particoes e coloca no index all_files[0]
            perror("nao abriu");
            break;
        }
        index[i] = *aux_prod;
        vet[i] = index[i].product_id;
        printf("\nValor: %d", index[i].product_id);
    }

    FILE* final = fopen("../output/fileprodsort.bin","wb");
    if(final == NULL){ 
        perror("Nao abriu o arquivo final");
        return 0;
    }
    int position;

    while (1) {
        position = find_small(vet, n_fragments);
        if (position < 0) {
            printf("\nsaiu pelo break");
            break;  //Sai do loop se não encontrar mais elementos válidos
        }

        *aux_prod = index[position];  //Copia dados de index para aux
        fwrite(aux_prod, sizeof(ProductEntry), 1, final);  //Escreve em final

        //Tenta ler do arquivo correspondente
        if (fread(aux_prod, sizeof(ProductEntry), 1, all_files[position]) != 1) {
            //Se a leitura falhar ou o arquivo acabou
            vet[position] = 0;  //Marca como processado
            printf("\nParticao %d acabou.", position);
        } else {
            //Se a leitura foi bem-sucedida
            index[position] = *aux_prod;  //Atualiza index
            vet[position] = aux_prod->product_id;  //Atualiza vet
        }
    }

    fclose(final);
    if (close_all(all_files, n_fragments) == 1){
        printf("Fechou\n");
    } 
}

int fileuser_merge(int n_fragments){
    FILE **all_files = open_parts(n_fragments);
    int *vet = (int*)malloc(sizeof(int) * n_fragments);
    UserEntry *aux_user = (UserEntry*)malloc(sizeof(UserEntry));
    UserEntry *index = (UserEntry*)malloc(sizeof(UserEntry)*n_fragments);
    
    for(int i = 0; i < n_fragments; i++){ //carrega as partições
        FILE *aux_file = all_files[i];
        if(fread(aux_user,sizeof(ProductEntry),1,aux_file) != 1){//le todo as particoes e coloca no index all_files[0]
            perror("nao abriu");
            break;
        }
        index[i] = *aux_user;
        vet[i] = index[i].user_id;
        printf("\nValor: %d", index[i].user_id);
    }

    FILE* final = fopen("../output/fileusersort.bin","wb");
    if(final == NULL){ 
        perror("Nao abriu o arquivo final");
        return 0;
    }
    int position;

    while (1) {
        position = find_small(vet, n_fragments);
        if (position < 0) {
            printf("\nsaiu pelo break");
            break;  //Sai do loop se não encontrar mais elementos válidos
        }

        *aux_user = index[position];  //Copia dados de index para aux
        fwrite(aux_user, sizeof(UserEntry), 1, final);  //Escreve em final

        //Tenta ler do arquivo correspondente
        if (fread(aux_user, sizeof(UserEntry), 1, all_files[position]) != 1) {
            //Se a leitura falhar ou o arquivo acabou
            vet[position] = 0;  //Marca como processado
            printf("\nParticao %d acabou.", position);
        } else {
            //Se a leitura foi bem-sucedida
            index[position] = *aux_user;  //Atualiza index
            vet[position] = aux_user->user_id;  //Atualiza vet
        }
    }

    fclose(final);
    if (close_all(all_files, n_fragments) == 1){
        printf("Fechou\n");
    }
}