#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALBUMS 10
#define MAX_PHOTOS 50
#define FILENAME "albums.txt"

#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"

typedef struct {
    char name[50];
    int size;
} Photo;

typedef struct {
    char name[50];
    Photo photos[MAX_PHOTOS];
    int photoCount;
} Album;

Album albums[MAX_ALBUMS];
int albumCount = 0;

void clearScreen() {
    printf("\033[H\033[J");
}

void header() {
    printf(COLOR_CYAN "--------------\n" COLOR_RESET);
    printf(COLOR_YELLOW "1" COLOR_RESET " - Create Album\n");
    printf(COLOR_YELLOW "2" COLOR_RESET " - Add Photo\n");
    printf(COLOR_YELLOW "3" COLOR_RESET " - Display Albums\n");
    printf(COLOR_YELLOW "4" COLOR_RESET " - Delete Album\n");
    printf(COLOR_YELLOW "0" COLOR_RESET " - Exit\n");
    printf(COLOR_CYAN "--------------\n" COLOR_RESET);
}

void loadAlbums() {
    FILE *file = fopen(FILENAME, "r");
    if (file != NULL) {
        while (fgets(albums[albumCount].name, 50, file) != NULL) {
            albums[albumCount].name[strcspn(albums[albumCount].name, "\n")] = '\0';
            fscanf(file, "%d", &albums[albumCount].photoCount);
            fgetc(file); 
            for (int i = 0; i < albums[albumCount].photoCount; i++) {
                fgets(albums[albumCount].photos[i].name, 50, file);
                albums[albumCount].photos[i].name[strcspn(albums[albumCount].photos[i].name, "\n")] = '\0';
                fscanf(file, "%d", &albums[albumCount].photos[i].size);
                fgetc(file);
            }
            albumCount++;
        }
        fclose(file);
    }
}

void saveAlbums() {
    FILE *file = fopen(FILENAME, "w");
    if (file != NULL) {
        for (int i = 0; i < albumCount; i++) {
            fprintf(file, "%s\n", albums[i].name);
            fprintf(file, "%d\n", albums[i].photoCount);
            for (int j = 0; j < albums[i].photoCount; j++) {
                fprintf(file, "%s\n", albums[i].photos[j].name);
                fprintf(file, "%d\n", albums[i].photos[j].size);
            }
        }
        fclose(file);
    }
}

void createAlbum() {
    clearScreen();
    if (albumCount >= MAX_ALBUMS) {
        printf(COLOR_RED "Album limit reached!\n" COLOR_RESET);
        return;
    }
    printf("Enter album name: ");
    fgets(albums[albumCount].name, 50, stdin);
    albums[albumCount].name[strcspn(albums[albumCount].name, "\n")] = '\0';
    albums[albumCount].photoCount = 0;
    albumCount++;
    saveAlbums();
    printf(COLOR_GREEN "Album created successfully!\n" COLOR_RESET);
}

void addPhoto() {
    clearScreen();
    if (albumCount == 0) {
        printf(COLOR_RED "No albums available. Create one first.\n" COLOR_RESET);
        return;
    }

    char albumName[50];
    printf("Enter album name: ");
    fgets(albumName, 50, stdin);
    albumName[strcspn(albumName, "\n")] = '\0';
    
    int index = -1;
    for (int i = 0; i < albumCount; i++) {
        if (strcmp(albums[i].name, albumName) == 0) {
            index = i;
            break;
        }
    }
    
    if (index == -1) {
        printf(COLOR_RED "Album not found! Returning to menu.\n" COLOR_RESET);
        return;
    }
    
    if (albums[index].photoCount >= MAX_PHOTOS) {
        printf(COLOR_RED "Photo limit reached in this album!\n" COLOR_RESET);
        return;
    }
    
    printf("Enter photo name: ");
    fgets(albums[index].photos[albums[index].photoCount].name, 50, stdin);
    albums[index].photos[albums[index].photoCount].name[strcspn(albums[index].photos[albums[index].photoCount].name, "\n")] = '\0';
    printf("Enter photo size (in KB): ");
    scanf("%d", &albums[index].photos[albums[index].photoCount].size);
    getchar();
    albums[index].photoCount++;
    saveAlbums();
    printf(COLOR_GREEN "Photo added successfully!\n" COLOR_RESET);
}

void displayAlbums() {
    clearScreen();
    if (albumCount == 0) {
        printf(COLOR_RED "No albums available!\n" COLOR_RESET);
        return;
    }
    for (int i = 0; i < albumCount; i++) {
        printf("Album [%d]: " COLOR_MAGENTA "%s" COLOR_RESET "\n", i, albums[i].name);
        int totalSize = 0;
        for (int j = 0; j < albums[i].photoCount; j++) {
            printf("Photo: " COLOR_BLUE "%s" COLOR_RESET " (" COLOR_GREEN "%d KB" COLOR_RESET ")\n", 
                   albums[i].photos[j].name, albums[i].photos[j].size);
            totalSize += albums[i].photos[j].size;
        }
        printf("Total Album Size: " COLOR_YELLOW "%d KB" COLOR_RESET "\n", totalSize);
    }
}

void deleteAlbum() {
    clearScreen();
    if (albumCount == 0) {
        printf(COLOR_RED "No albums to delete!\n" COLOR_RESET);
        return;
    }

    char albumName[50];
    printf("Enter album name to delete: ");
    fgets(albumName, 50, stdin);
    albumName[strcspn(albumName, "\n")] = '\0';

    int index = -1;
    for (int i = 0; i < albumCount; i++) {
        if (strcmp(albums[i].name, albumName) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf(COLOR_RED "Album not found! Returning to menu.\n" COLOR_RESET);
        return;
    }

    for (int i = index; i < albumCount - 1; i++) {
        albums[i] = albums[i + 1];
    }
    albumCount--;
    saveAlbums();
    printf(COLOR_GREEN "Album \"" COLOR_MAGENTA "%s" COLOR_GREEN "\" deleted successfully!\n" COLOR_RESET, albumName);
}

void menu(int op) {
    switch (op) {
        case 1:
            createAlbum();
            break;
        case 2:
            addPhoto();
            break;
        case 3:
            displayAlbums();
            break; 
        case 4:
            deleteAlbum();
            break;
        default:
            printf("Exiting application.\n");
    }
}

int main() {
    loadAlbums();
    
    int option;
    do {
        clearScreen();
        header();
        printf("Enter option: ");
        scanf("%d", &option);
        getchar();
        menu(option);
        printf("Press Enter to continue...");
        getchar();
    } while (option > 0 && option <= 4);

    return 0;
}