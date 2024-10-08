#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_TITLE_LENGTH 80
#define MAX_LINK_LENGTH 100
#define MAX_DIRECTOR_LENGTH 50

typedef struct {
    int id;
    char name[MAX_DIRECTOR_LENGTH];
} Director;

typedef struct {
    int id;
    char title[MAX_TITLE_LENGTH];
    int releaseYear;
    char url[MAX_LINK_LENGTH];
    int directorID;
} Movie;

void print_movie(const Movie* movie) {
    printf("ID: %d\n", movie->id);
    printf("Title: %s\n", movie->title);
    printf("Release Year: %d\n", movie->releaseYear);
    printf("URL: %s\n", movie->url);
    printf("Director ID: %d\n", movie->directorID);
}

void print_director(const Director* director) {
    printf("Name: %s\n", director->name);
    printf("\n");
}

void read_movies(const char* filename, Movie** movies, int* movie_count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo arqMovies.txt");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    *movie_count = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        (*movie_count)++;
    }

    rewind(file);

    *movies = (Movie*)malloc(*movie_count * sizeof(Movie));
    if (*movies == NULL) {
        perror("Erro na alocação de memória para filmes");
        exit(1);
    }

    int i = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; 
        char* token = strtok(line, ";");
        if (token != NULL) {
            (*movies)[i].id = atoi(token);
            token = strtok(NULL, ";");
        }
        if (token != NULL) {
            strncpy((*movies)[i].title, token, sizeof((*movies)[i].title));
            (*movies)[i].title[sizeof((*movies)[i].title) - 1] = '\0';
            token = strtok(NULL, ";");
        }
        if (token != NULL) {
            (*movies)[i].releaseYear = atoi(token);
            token = strtok(NULL, ";");
        }
        if (token != NULL) {
            strncpy((*movies)[i].url, token, sizeof((*movies)[i].url));
            (*movies)[i].url[sizeof((*movies)[i].url) - 1] = '\0';
            token = strtok(NULL, ";");
        }
        if (token != NULL) {
            (*movies)[i].directorID = atoi(token);
        }
        i++;
    }

    fclose(file);
}

void read_directors(const char* filename, Director** directors, int* director_count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo directors.txt");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    *director_count = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        (*director_count)++;
    }

    rewind(file);

    *directors = (Director*)malloc(*director_count * sizeof(Director));
    if (*directors == NULL) {
        perror("Erro na alocação de memória para diretores");
        exit(1);
    }

    int i = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; //Remove o \0
        char* token = strtok(line, ",");
        if (token != NULL) {
            (*directors)[i].id = atoi(token);
            token = strtok(NULL, ",");
        }
        if (token != NULL) {
            strncpy((*directors)[i].name, token, sizeof((*directors)[i].name));
            (*directors)[i].name[sizeof((*directors)[i].name) - 1] = '\0';
        }
        i++;
    }

    fclose(file);
}

void match_movies_and_directors(Movie* movies, int movie_count, Director* directors, int director_count) {
    for (int i = 0; i < movie_count; i++) {
        Movie* movie = &movies[i];
        int found_director = 0;

        for (int j = 0; j < director_count; j++) {
            Director* director = &directors[j];
            if (movie->directorID == director->id) {
                printf("Movie and Director match found:\n");
                print_movie(movie);
                print_director(director);
                printf("\n");
                found_director = 1;
                break;
            }
        }

        if (!found_director) {
            printf("Movie ID %d has no matching director.\n", movie->id);
        }
    }
}

int main() {
    Movie* movies = NULL;
    Director* directors = NULL;
    int movie_count = 0, director_count = 0;

    read_movies("arqMovies.txt", &movies, &movie_count);
    read_directors("directors.txt", &directors, &director_count);

    match_movies_and_directors(movies, movie_count, directors, director_count);

    free(movies);
    free(directors);

    return 0;
}
