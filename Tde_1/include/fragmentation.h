#ifndef _FRAGMENTATION_H_
#define _FRAGMENTATION_H_

#include "reader.h"

int file_sort_frag(FILE *input, size_t data_size, int (*compare)(const void *, const void*));

#endif