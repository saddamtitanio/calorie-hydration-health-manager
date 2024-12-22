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
#define HEALTH_FILE "health.txt"

char *getDbFilePath(const char *file) {
    int filePathSize = strlen(DATABASE_DIR) + strlen("/") + strlen(file);
    char *filePath = malloc(filePathSize + 1);
    
    if (filePath == NULL) {
        return NULL;
    }    
    snprintf(filePath, filePathSize + 1, "%s/%s", DATABASE_DIR, file);

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
            return false;
        }
    }
    return true;
}

void createDbFile(const char *fileName) {
    createDbDirectory();
    char *filePath = getDbFilePath(fileName);
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
