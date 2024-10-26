#ifndef _READER_H_
#define _READER_H_

#include <stdio.h>

typedef struct product_entry{
    int product_id;
    float price;
    unsigned long long category_id;
    char category_code[50];
    char brand[16]; 
} ProductEntry;

typedef struct user_entry{
    long long unsigned user_id;
    char event_time[24];
    char event_type[17];
    char user_session[37];
} UserEntry;

int file_reader(FILE *source);

#endif