#include <stdlib.h>
#include <string.h>

#include "../include/reader.h"
#include ""

read_file(FILE *input, size_t model){
    NODO *root, *last;

    int lst_value = 0;
    int adress = 0;

    void *buffer = malloc(model);

    while (fread(buffer, model, 1, input) == 1){

        if (root == NULL){ // inicia a linked list
            aux = create_nodo(key);
            root = aux;
        }

        else{
            if(())

        }
        adress++;
    }

    return root;
}

NODO * create_nodo(int key){
    NODO *aux = (NODO *)malloc(sizeof(NODO *));
    aux->next = NULL;
    aux->key = key;
    aux->
}