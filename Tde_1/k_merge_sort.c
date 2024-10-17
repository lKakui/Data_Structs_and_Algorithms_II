#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 8192 // Tamanho do buffer
#define NUM_PARTES 16    // Número de partes para fragmentação

typedef struct produto {
    int idProd;
    float price;
    char brand[30];
    char codeCategory[40];
} PROD;

typedef struct cliente {
    int idUser;
    char eventType[10];
    char session[50];
} CLIENTE;

long fsize(FILE *f) {
    fseek(f, 0, SEEK_END);
    long file_size = ftell(f);
    rewind(f);
    return file_size;
}

int compareprod(const void *a, const void *b) {// comparador usando idProd
    PROD *prodA = (PROD *)a;
    PROD *prodB = (PROD *)b;
    return prodA->idProd - prodB->idProd; 
}

int compareclient(const void *a, const void *b) {// comparador usando iduser
    CLIENTE *clientA = (CLIENTE *)a;
    CLIENTE *clientB = (CLIENTE *)b;
    return clientA->idUser - clientB->idUser;
}

void sort_part(const char *filename, int flag) {
    FILE *arquivo = fopen(filename, "rb+");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return;
    }

    long num_registros = fsize(arquivo);
    if (num_registros <= 0) {
        fclose(arquivo);
        return;
    }

    long registro_count;
    if (flag == 0) { // Produtos
        registro_count = num_registros / sizeof(PROD);
        PROD *produtos = malloc(registro_count * sizeof(PROD));
        if (!produtos) {
            perror("Erro ao alocar memória");
            fclose(arquivo);
            return;
        }

        fread(produtos, sizeof(PROD), registro_count, arquivo);
        qsort(produtos, registro_count, sizeof(PROD), compareprod);
        rewind(arquivo);
        fwrite(produtos, sizeof(PROD), registro_count, arquivo);
        free(produtos);
    } else { // Clientes
        registro_count = num_registros / sizeof(CLIENTE);
        CLIENTE *clients = malloc(registro_count * sizeof(CLIENTE));
        if (!clients) {
            perror("Erro ao alocar memória");
            fclose(arquivo);
            return;
        }

        fread(clients, sizeof(CLIENTE), registro_count, arquivo);
        qsort(clients, registro_count, sizeof(CLIENTE), compareclient);
        rewind(arquivo);
        fwrite(clients, sizeof(CLIENTE), registro_count, arquivo);
        free(clients);
    }

    fclose(arquivo);
}

void frag_arq(FILE *arquivo) {
    PROD product;
    CLIENTE client;

    int c = 0, index = 0;
    char buffer[256];
    char filename[20];
    FILE *prodpart_file[NUM_PARTES], *clientpart_file[NUM_PARTES];
    long file_size = fsize(arquivo);
    long total_records = 0;

    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        total_records++;
    }
    rewind(arquivo);
    size_t part_size = total_records / NUM_PARTES; //tamanho das partições

    // Inicializa arquivos de fragmentação
    for (int i = 0; i < NUM_PARTES; i++) {
        sprintf(filename, "prodpart%d.bin", i + 1);
        prodpart_file[i] = fopen(filename, "wb");
        if (prodpart_file[i] == NULL) {
            perror("Erro ao criar arquivo de produtos");
            exit(1);
        }
        sprintf(filename, "clientpart%d.bin", i + 1);
        clientpart_file[i] = fopen(filename, "wb");
        if (clientpart_file[i] == NULL) {
            perror("Erro ao criar arquivo de clientes");
            exit(1);
        }
    }

    // Lê o arquivo de entrada e fragmenta
    while (fgets(buffer, sizeof(buffer), arquivo) != NULL) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove nova linha

        // Parseando os dados
        char *token = strtok(buffer, ",");
        if (token) {
            strncpy(client.eventType, token, sizeof(client.eventType) - 1);
            client.eventType[sizeof(client.eventType) - 1] = '\0';
            token = strtok(NULL, ",");
            product.idProd = atoi(token);
            token = strtok(NULL, ",");
            product.price = strtof(token, NULL);
            token = strtok(NULL, ",");
            client.idUser = atoi(token);
            token = strtok(NULL, ",");
            strncpy(client.session, token, sizeof(client.session) - 1);
            client.session[sizeof(client.session) - 1] = '\0';

            // Ajuster se não há valor
            strncpy(product.codeCategory, "null", sizeof(product.codeCategory) - 1);
            strncpy(product.brand, "null", sizeof(product.brand) - 1);

            // Grava os dados nas partições
            fwrite(&product, sizeof(PROD), 1, prodpart_file[index]);
            fwrite(&client, sizeof(CLIENTE), 1, clientpart_file[index]);

            c++;
            // Verifica se está cheio o arquivo
            if (c >= part_size && index < NUM_PARTES - 1) {
                c = 0;
                fclose(prodpart_file[index]);
                fclose(clientpart_file[index]);
                index++;
            }
        }
    }

    // Fecha todos os arquivos restantes
    for (int i = 0; i < NUM_PARTES; i++) {
        if (prodpart_file[i] != NULL) {
            fclose(prodpart_file[i]);
        }
        if (clientpart_file[i] != NULL) {
            fclose(clientpart_file[i]);
        }
    }
}

void process_Arq(FILE *teste) {
    frag_arq(teste);
    for (int i = 0; i < NUM_PARTES; i++) {
        char filename[20];
        sprintf(filename, "prodpart%d.bin", i + 1);
        sort_part(filename, 0);
        sprintf(filename, "clientpart%d.bin", i + 1);
        sort_part(filename, 1);
    }
}

int main() {
    FILE *f = fopen("C:\\Users\\Thiago Spiringer\\Documents\\Ucs\\data_structII\\Tde_1\\2019-Nov.csv", "r");
    if (f == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }
    process_Arq(f);
    fclose(f);
    return 0;
}
