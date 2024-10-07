#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool int

struct movie{
	int id;
	char nome[64];
	int age;
	char link[100];
	int id_direc;
};

struct index{
    int id;
    int deslocamento;
    bool on;
    bool extra;
    int prox ;
};

void txt2bin(FILE *filme, FILE *filmebin){
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
        
        fwrite(novo, sizeof(struct movie),1,filmebin); // fwrite usado quando o arq Ã© binario, (end struct, tamanho do dado, qntd de elementos, arq. destino)
	}
	
	free(novo);
	
	fclose(filmebin);
}

int calcsize(FILE *bin){
    int aux;
    fseek(bin, 0, SEEK_END);
    rewind(bin);
    return aux;
}

struct index * orgarq(FILE *bin, int size){
    int i;
    struct movie *aux;
    struct index *root = (struct index *)calloc(0,size*sizeof(struct index));
    for(i=0; i < size+1; i++){
        fread(&aux, sizeof(struct movie), 1, bin);
        root[aux->id].id = aux->id;
        root[i].deslocamento = i;
    }
    return root;
}


void menufilmes(){
    int size;
    struct index *root;
    FILE *txt = fopen("arqMovies.txt", "r");
    FILE *bin = fopen("arqMovies.bin", "a+");
    if(bin == NULL){
        bin = fopen("arqMovies.bin", "wb+");
        fclose(bin);
        bin = fopen("arqMovies.bin", "a+");
    }

    txt2bin(txt, bin);

    size = calcsize(bin);

    root = orgarq(bin, size);



}






int main(){
    menufilmes();
}
