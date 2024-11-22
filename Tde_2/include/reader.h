#ifndef _READER_H_
#define _READER_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct node{
    int key;
    int adress;
    struct node *next;
}NODO;

typedef struct product{
    int id_product;
    float price;
    unsigned long long category_id[50];
    char category_desc;
    char brand[16];
}PRODUCT;




#endif 