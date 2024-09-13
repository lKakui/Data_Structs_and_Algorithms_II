#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct movie{
	int id;
	char nome[64];
	int age;
	char link[100];
	int id_direc;
};

struct dicio{
	int val;
	char key;
	struct dicio *prox;
};

int read_archbin(FILE *filme, struct dicio **ini){
	int count = 0;
	char letra = '\0';

	struct dicio *aux = *ini;
	struct movie read;

	while (fread(&read, sizeof(struct movie), 1, filme)) {
        if (read.nome[0] > letra || letra == '\0') {
            struct dicio *novo = (struct dicio *)malloc(sizeof(struct dicio));
            if (novo == NULL) {
                perror("Erro ao alocar memória");
                exit(1);
            }
            novo->key = read.nome[0];
            novo->val = count;
            novo->prox = NULL;

            letra = read.nome[0]; // Atualize a letra depois de adicionar o novo item

            if (*ini == NULL) { // Primeira inserção
                *ini = novo;
                aux = novo;
            } else {
                aux->prox = novo;
                aux = novo;
            }
        }
        count++;
    }

	rewind(filme); //rebobina o arquivo
	
	return 0;
}

void txt2bin(FILE *filme){

	FILE *filmebin = fopen("arqMovies.bin", "wb"); //abri o arquivo em modo de escrita se n?o existe ele cria
	if(filmebin == NULL){
		printf("Erro ao abrir o bin");
		return;
	}
	
	struct movie *novo = (struct movie*)malloc(sizeof(struct movie));
	
	char line[200];
	
	while((fgets(line, sizeof(line),filme))!= NULL){
		line[strcspn(line, "\n")] = '\0'; //percorre a linha ate achar o \n, coloca nesta posi??o o \0
		char* token = strtok(line, ","); //token recebe a string em line at? encontrar o ";";
		if(token != NULL){
			novo->id = atoi(token); //converte o string id em int
			token = strtok(NULL, ","); //o null fala para a fun?ao continuar, buscando valores da string mandado anteriormentem, na qual agora ira enviar o segundo token
		}
		if(token != NULL){
			strncpy(novo->nome, token, sizeof(novo->nome)); //copia a string em token e cola na struct 
			novo->nome[sizeof(novo->nome) - 1] = '\0'; //coloca o \0 no final para delimitar a string
			token = strtok(NULL, ","); //recebe o pr?ximo token
		}
		if(token != NULL){
			novo->age = atoi(token); //converte o string age em int
			token = strtok(NULL, ",");
		}
		if(token != NULL){
			strncpy(novo->link, token, sizeof(novo->link)); //copia a string em token e cola na struct 
			novo->link[sizeof(novo->link) - 1] = '\0'; //coloca o \0 no final para delimitar a string
			token = strtok(NULL, ","); //recebe o pr?ximo token
		}
		if (token != NULL) {
            novo->id_direc = atoi(token);
        }
        
        fwrite(novo, sizeof(struct movie),1,filmebin); // fwrite usado quando o arq é binario, (end struct, tamanho do dado, qntd de elementos, arq. destino)
	}
	
	free(novo);
	
	fclose(filmebin);
}

void print_dicio(struct dicio *ini){
struct dicio *aux = ini; //neste caso o endereço vai pegar o valor e colocar no endereço
printf("\nLista: \n");
while(aux != NULL){
printf("%d %c (%p) \n",aux->val,aux->key, aux);
aux= aux->prox;
}
printf("\n----------------------------*");
}

int main(){
	
	FILE *filme = fopen("arqMovies.bin", "rb");
    if (filme == NULL) {
        perror("Erro ao abrir o arquivo de texto");
        return 1;
    }
	
	//txt2bin(filme);

	struct dicio *ini = NULL;
	
	read_archbin(filme, &ini);

	print_dicio(ini);
	
	//search_arq(filme);	
}
