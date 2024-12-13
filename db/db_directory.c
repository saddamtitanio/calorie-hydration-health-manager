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

char *getDbFilePath() {
    int filePathSize = strlen(DATABASE_DIR) + strlen("/") + strlen(USER_FILE);
    char *filePath = malloc(filePathSize + 1);
    
    if (filePath == NULL) {
        return NULL;
        exit(EXIT_FAILURE);
    }    
    snprintf(filePath, filePathSize + 1, "%s/%s", DATABASE_DIR, USER_FILE);

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

void createDbFile() {
    createDbDirectory();
    char *filePath = getDbFilePath();
    FILE *file = fopen(filePath, "a");

    if (file == NULL) {
        perror("Error creating database file");
        free(filePath);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    free(filePath);
}

void closeDBFile(FILE **file, char **filePath) {
    if (*file) {
        fclose(*file);
        *file = NULL;
    }
    if (*filePath) {
        free(*filePath);
        *filePath = NULL;
    }
}
