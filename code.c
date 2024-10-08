#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct produto {
    float price;
    char idProd[20];      // Primary Key
    char brand[30];
    char codeCategory[40];
} PROD;

typedef struct cliente {
    char eventType[10];
    char idUser[15];      // Primary Key
    char session[50];
} CLIENTE;

void preencher_string(char *str, int tamanho) {
    if (strlen(str) == 0) {
        strncpy(str,"nulo", tamanho - 1);
        str[tamanho - 1] = '\0'; 
    }
    
    // preencher a string com espaço em branco para deixa tudo de um tamanho especifico
    int len = strlen(str);
    for (int i = len; i < tamanho - 1; i++) {
        str[i] = ' ';
    }
    str[tamanho - 1] = '\0'; 
}

void ler_arquivo(FILE *f) { // f -> arquivo de entrada 
    FILE *arqProduto = fopen("Arquivo_Produto.bin", "wb");
    FILE *arqCliente = fopen("Arquivo_Cliente.bin", "wb");

    if (!arqProduto || !arqCliente) {
        perror("Erro ao abrir arquivos");
        return;
    }
    
    char linha[200];
    PROD produto;
    CLIENTE cliente;

    // Ler cada linha do CSV
    while (fscanf(f, "%s",linha) == 1 ){
        char *token = strtok(linha,",");
        //verificar a ordem dos tokens, para validar certo, não deve estar certo e nem na ordem certa
        //Copiar os token para as structs  //confirir isso depois
        strncpy(cliente.idUser , token[0]); //arrumar 
        strncpy(produto.idProd , token[1]); //arrumar 
        strncpy(produto.codeCategory , token[2]); //arrumar 
        strncpy(produto.brand , token[3]); //arrumar 
        strncpy(cliente.eventType , token[4]); //arrumar 
        strncpy(cliente.session , token[5]); //arrumar 

        preencher_string(cliente.idUser, sizeof(cliente.idUser));
        preencher_string(produto.idProd, sizeof(produto.idProd));
        preencher_string(produto.codeCategory, sizeof(produto.codeCategory));
        preencher_string(produto.brand, sizeof(produto.brand));
        preencher_string(cliente.eventType, sizeof(cliente.eventType));
        preencher_string(cliente.session, sizeof(cliente.session));

        // Escrever no arquivo de produtos
        fwrite(&produto, sizeof(PROD), 1, arqProduto);

        // Escrever no arquivo de clientes
        fwrite(&cliente, sizeof(CLIENTE), 1, arqCliente);
    }
    fclose(arqProduto);
    fclose(arqCliente);
}

int cmpCliente(CLIENTE i, CLIENTE j){
    return strcmp(i.idUser,j.idUser); // 1 se i é maior, -1 se 2 é maior
                                        // 0 se é igual
}

int cmpProd(PROD i,PROD j){
    return strcmp(i.idProd,j.idProd); // 1 se i é maior, -1 se 2 é maior
                                        // 0 se é igual
}

