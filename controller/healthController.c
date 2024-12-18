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
        if (atoi(buffer) == id) {
            return line;
        }
    }
    return NULL;


}


Progress setHealthProfile(int id) {
    char *profile = readHealthProfile(id);
    char *token = strtok(profile, "|");
    Progress progress;
    int tokenCount = 0;
    while (token != NULL) {
        if (tokenCount == 0) {
            progress.userId = atoi(token);
        }
        else if (tokenCount == 1) {
            progress.targetWeight = atof(token);
        }
        else if (tokenCount == 2) {
            progress.targetDays = atoi(token);
        }
        else if (tokenCount == 3) {
            progress.lifestyle = atoi(token);
        }
        tokenCount++;
        token = strtok(NULL, "|");
    }
    return progress;
}


void activityLevelMultiplier(float *BMR, Lifestyle lifestyle) {
    switch (lifestyle) {
        case SEDENTARY:
            *BMR *= 1.2;
            break;
        case LIGHTLY_ACTIVE:
            *BMR *= 1.375;
            break;
        case MODERATELY_ACTIVE:
            *BMR *= 1.55;
            break;
        case VERY_ACTIVE:
            *BMR *= 1.725;
            break;
        case EXTRA_ACTIVE:
            *BMR *= 1.9;
            break;
        default:
            printf("Invalid BMR");
            break;
    }
}


float calculateBMR(User *user, Progress progress) {
    float BMR = 10 * user->weight + 6.25 * user->height - 5 * user->age;
    if (user->sex == 1) {
        BMR -= 5;
    }
    else {
        BMR -= 161;
    }
    activityLevelMultiplier(&BMR, progress.lifestyle);


    return BMR;
}
