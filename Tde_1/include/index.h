#ifndef _INDEX_H_
#define _INDEX_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct partial{
    int key;
    int end;
}PART_INDEX;

int create_partial_index(FILE *input, size_t data_size, int flag);

int bin_search_index(FILE* index, FILE* input , int key);

int find_in_index(FILE * input, size_t data_size, int key);

#endif