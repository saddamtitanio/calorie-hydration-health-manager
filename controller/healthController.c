#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/utils.h"
#include "../db/db_directory.h"
#include "usersController.h"

void writeHealthFile(int userId, float targetWeight, int targetDays) {
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
    
    snprintf(buffer, 256, "%d|%.2f|%d\n", userId, targetWeight, targetDays);
    fprintf(file, buffer);
    closeDBFile(&file, &filePath);
}