#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/reader.h"

int file_reader(FILE *data){
    FILE *userfile = fopen("../output/userfile.bin", "wb");
    FILE *prodfile = fopen("../output/prodfile.bin", "wb");

    if(userfile == NULL || prodfile == NULL){
        printf("deu problema reader\n");
        perror("erro ao abrir o arquivo");
    }

    UserEntry user_buffer;
    ProductEntry product_buffer;

    char buffer[256];
    
    fgets(buffer, sizeof(buffer), data); //pula a primeira linha

    clock_t start = clock();

    while (fgets(buffer, sizeof(buffer), data) != NULL){
        sscanf(buffer, "%[^,],%[^,],%d,%lld,%[^,],%[^,],%f,%d,%[^,]",
            user_buffer.event_time,
            user_buffer.event_type,
            &product_buffer.product_id,
            &product_buffer.category_id,
            product_buffer.category_code,
            product_buffer.brand,
            &product_buffer.price,
            &user_buffer.user_id,
            user_buffer.user_session);

        fwrite(&user_buffer,sizeof(user_buffer), 1, userfile);
        fwrite(&product_buffer,sizeof(product_buffer), 1, prodfile);

        memset(&user_buffer,0, sizeof(user_buffer));
        memset(&product_buffer,0,sizeof(product_buffer));
    }

    fclose(userfile);
    fclose(prodfile);
}