#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/utils.h"
#include "../db/db_directory.h"
#include "usersController.h"

void writeHealthFile(int userId, float targetWeight, int targetDays, Lifestyle lifestyle) {
    char *filePath = getDbFilePath(HEALTH_FILE);
    char buffer[256];

    if (filePath == NULL) {
        perror("Failed to get path.");
        return;
    }

    FILE *file = fopen(filePath, "a");

    if (file == NULL) {
        perror("Error opening file");
        free(filePath);
        return; 
    }
    
    snprintf(buffer, 500, "%d|%.2f|%d|%d\n", userId, targetWeight, targetDays, lifestyle);
    fprintf(file, buffer);
    closeDBFile(&file, &filePath);
}

/* Check whether the user has previously set a goal or not */
bool isNullHealthProfile(int id) {
    char *filePath = getDbFilePath(HEALTH_FILE);
    char buffer[2];

    if (filePath == NULL) {
        perror("Failed to get path.");
        return false;
    }

    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        perror("Error opening file");
        free(filePath);
        return false; 
    }
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char *token = strtok(buffer, "|");
        if (atoi(token) == id) {
            return false;
        }
        token = strtok(NULL, "|");
    }

    closeDBFile(&file, &filePath);
    return true;
}

char *readHealthProfile(int id) {
    char *filePath = getDbFilePath(HEALTH_FILE);
    char buffer[256];

    if (filePath == NULL) {
        perror("Failed to get path.");
        return NULL;
    }

    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        perror("Error opening file");
        free(filePath);
        return NULL; 
    }
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char *line = buffer;
        char *token = strchr(line, '|');
        
        if (token) {
            if (atoi(buffer) == id) {
                return line;
            }
        }
    }
    return NULL;

}