#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>

/* Compatibility with Windows OS and POSIX systems*/
#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#endif

#define DATABASE_DIR "database"
#define USER_FILE "user.txt"

typedef struct {
    int id;
    char name[50];
    int age;
    float height;
    float weight;
    char datetime[50];
} User;

bool createDbDirectory();
void displayTitleMenu();
char *getDbFilePath(const char *filename);
void createDbFile(const char *filename);

int main(void) {
    createDbDirectory();
    createDbFile(USER_FILE);

    return 0;
}

void createDbFile(const char *filename) {
    char *filePath = getDbFilePath(filename);
    FILE *file = fopen(filePath, "w+");

    if (file == NULL) {
        perror("Error creating database file");
        free(filePath);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    free(filePath);
}

char *getDbFilePath(const char *filename) {
    char *filePath = malloc(sizeof(char) * strlen(DATABASE_DIR) + strlen("/") + strlen(filename) + 1);
    
    if (filePath == NULL) {
        exit(EXIT_FAILURE);
    }
    strcpy(filePath, DATABASE_DIR);
    strcat(filePath, "/");
    strcat(filePath, USER_FILE);

    return filePath;
}

bool createDbDirectory() {
    const char *folder = DATABASE_DIR;
    struct stat sb;

    if (stat(folder, &sb) == -1) {
        if (mkdir(DATABASE_DIR, 0777) == 0) {
            return true;
        } else {
            perror("Error creating directory.");
            exit(EXIT_FAILURE);
        }
    }
    return true;
}