#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 10
#define MAX_TOKEN_SIZE 50

typedef struct produto {
    float price;
    int idProd;
    char brand[30];
    char codeCategory[40];
} PROD;

typedef struct cliente {
    char eventType[10];
    int idUser;
    char session[50];
} CLIENTE;

int split_line_custom(const char *line, char **tokens, int max_tokens) {
    int count = 0;
    const char *start = line;
    const char *ptr = line;
    char buffer[MAX_TOKEN_SIZE];
    
    while (*ptr != '\0' && count < max_tokens) {
        if (*ptr == ',') {
            size_t len = ptr - start;
            if (len >= MAX_TOKEN_SIZE) len = MAX_TOKEN_SIZE - 1;
            strncpy(buffer, start, len);
            buffer[len] = '\0';
            tokens[count] = malloc(MAX_TOKEN_SIZE * sizeof(char));
            if (tokens[count] == NULL) {
                perror("Erro ao alocar memória para token");
                exit(EXIT_FAILURE);
            }
            strncpy(tokens[count], buffer, MAX_TOKEN_SIZE - 1);
            tokens[count][MAX_TOKEN_SIZE - 1] = '\0';
            count++;
            start = ptr + 1;
        }
        ptr++;
    }
    if (count < max_tokens) {
        size_t len = ptr - start;
        if (len >= MAX_TOKEN_SIZE) len = MAX_TOKEN_SIZE - 1;
        strncpy(buffer, start, len);
        buffer[len] = '\0';
        tokens[count] = malloc(MAX_TOKEN_SIZE * sizeof(char));
        if (tokens[count] == NULL) {
            perror("Erro ao alocar memória para token");
            exit(EXIT_FAILURE);
        }
        strncpy(tokens[count], buffer, MAX_TOKEN_SIZE - 1);
        tokens[count][MAX_TOKEN_SIZE - 1] = '\0';
        count++;
    }
    return count;
}

void free_tokens(char **tokens, int count) {
    for (int i = 0; i < count; i++) {
        free(tokens[i]);
    }
}

void ler_arquivo(FILE *f) {
    // Obter o tamanho do arquivo
    fseek(f, 0, SEEK_END); // Move o ponteiro para o final do arquivo
    long tamanho_arquivo = ftell(f); // Captura a posição atual do ponteiro (tamanho do arquivo)
    rewind(f);

    printf("Tamanho do arquivo: %ld bytes\n", tamanho_arquivo); // Imprime o tamanho do arquivo

    FILE *arqProduto = fopen("Arquivo_Produto.bin", "wb");
    FILE *arqCliente = fopen("Arquivo_Cliente.bin", "wb");

    if (!arqProduto || !arqCliente) {
        perror("Erro ao abrir arquivos");
        if (arqProduto) fclose(arqProduto);
        if (arqCliente) fclose(arqCliente);
        return;
    }

    char linha[500];
    PROD produto;
    CLIENTE cliente;
    char *tokens[MAX_TOKENS];
    int count = 0;

    // Ler o cabeçalho (se houver)
    if (fgets(linha, sizeof(linha), f) == NULL) {
        printf("Arquivo vazio ou erro na leitura do cabeçalho.\n");
        fclose(arqProduto);
        fclose(arqCliente);
        return;
    }

    // Processar cada linha
    while (fgets(linha, sizeof(linha), f) != NULL) {
        linha[strcspn(linha, "\n")] = '\0';  // Remove nova linha
        int num_tokens = split_line_custom(linha, tokens, MAX_TOKENS);

        // Para depuração: imprime a linha lida
        printf("Linha lida: %s\n", linha);

        // Processa os dados do cliente
        if (num_tokens > 1 && tokens[1][0] != '\0') {
            strncpy(cliente.eventType, tokens[1], sizeof(cliente.eventType) - 1);
            cliente.eventType[sizeof(cliente.eventType) - 1] = '\0';
        } else {
            strcpy(cliente.eventType, "vazio");
        }

        // Processa os dados do produto
        if (num_tokens > 2 && tokens[2][0] != '\0') {
            produto.idProd = atoi(tokens[2]);
        } else {
            produto.idProd = 0;
        }

        if (num_tokens > 4 && tokens[4][0] != '\0') {
            strncpy(produto.codeCategory, tokens[4], sizeof(produto.codeCategory) - 1);
            produto.codeCategory[sizeof(produto.codeCategory) - 1] = '\0';
        } else {
            strcpy(produto.codeCategory, "vazio");
        }

        if (num_tokens > 5 && tokens[5][0] != '\0') {
            strncpy(produto.brand, tokens[5], sizeof(produto.brand) - 1);
            produto.brand[sizeof(produto.brand) - 1] = '\0';
        } else {
            strcpy(produto.brand, "vazio");
        }

        if (num_tokens > 6 && tokens[6][0] != '\0') {
            produto.price = atof(tokens[6]);
        } else {
            produto.price = 0.0;
        }

        if (num_tokens > 7 && tokens[7][0] != '\0') {
            cliente.idUser = atoi(tokens[7]);
        } else {
            cliente.idUser = 0;
        }

        if (num_tokens > 8 && tokens[8][0] != '\0') {
            strncpy(cliente.session, tokens[8], sizeof(cliente.session) - 1);
            cliente.session[sizeof(cliente.session) - 1] = '\0';
        } else {
            strcpy(cliente.session, "vazio");
        }

        // Grava os dados no arquivo binário
        fwrite(&produto, sizeof(PROD), 1, arqProduto);
        fwrite(&cliente, sizeof(CLIENTE), 1, arqCliente);

        // Imprime informações lidas
        printf("Linha %d:\n", count + 1);
        printf("Produto:\n");
        printf("ID: %d\n", produto.idProd);
        printf("Brand: %s\n", produto.brand);
        printf("Category Code: %s\n", produto.codeCategory);
        printf("Price: %.2f\n", produto.price);
        printf("\nCliente:\n");
        printf("User ID: %d\n", cliente.idUser);
        printf("Event Type: %s\n", cliente.eventType);
        printf("Session: %s\n\n", cliente.session);
        printf("Linha %d lida.\n\n", count + 1);

        count++;
        free_tokens(tokens, num_tokens);
    }

    printf("Leu %d linhas.\n", count);
    fclose(arqProduto);
    fclose(arqCliente);
}

int main() {
    FILE *f = fopen("2019-Nov.csv", "r");
    if (f == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return 1;
    }

    ler_arquivo(f);
    fclose(f);
    return 0;
}
