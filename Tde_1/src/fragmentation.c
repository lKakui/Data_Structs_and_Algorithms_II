#include <stdio.h>
#include <stdlib.h>

#include "reader.h"

#define FRAGMENT_SIZE 10

int file_sort_frag(FILE *input, size_t data_size, int (*compare)(const void *, const void*)){
    int count, iter = 0;
    void *buffer = malloc(data_size * FRAGMENT_SIZE);
    char file_name[35];

    while ((count = fread(buffer, data_size, FRAGMENT_SIZE, input)))
    {
        iter++;
        qsort(buffer, count, data_size, compare);
        sprintf(file_name, "../output/partitions/fragment_%d.bin", iter);
        FILE *output = fopen(file_name, "wb");
        fwrite(buffer, data_size, count, output);
        fclose(output);
    }

    free(buffer);
    return iter;
}



