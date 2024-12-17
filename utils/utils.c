#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"

void clearInputBuffer() {
    while(getchar() != '\n');
}

float calculateBMI(float height, float weight) {
    float normalizedHeight = height / 100;
    return (weight) / (normalizedHeight * normalizedHeight);
}

char *getCurrentLocalDateTime() {
    char *formattedDateTime = malloc(20 * sizeof(char));
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    strftime(formattedDateTime, 20, "%Y-%m-%d %H:%M:%S", localTime);
    
    return formattedDateTime;
}

bool rangeCheck(void *value, int dataType, const char *msg, bool *isValid) {
    if ((dataType == 0 && *(int *)value <= 0) || (dataType == 1 && *(float *)value <= 0.0f)) {
        *isValid = false;
        printf("%s\n\n", msg);
        clearInputBuffer();
        return false;
    }
    return true;
}

bool isDataValid(int scanf, bool *isValid, const char *msg) {
    if (!scanf) {
        *isValid = false;
        printf("%s\n\n", msg);
        clearInputBuffer();
        return false;
    }
    return true;
}

void showDetails(char *line, int *currentUserCount) {
    int tokenIndex = 0;
    char *token = strtok(line, "|");
    while (token != NULL) {
        if (tokenIndex == 1) {
            printf("| %-3d %-28s |\n", ++(*currentUserCount), token);
        }
        else if (tokenIndex == 2) {
            printf("| %-3s %s %-2s %-19d |\n", " ", "Age: ", " ", atoi(token));
        }
        else if (tokenIndex == 3) {
            printf("| %-3s %s %-16.2f cm |\n", " ", "Height: ", atof(token));
        }
        else if (tokenIndex == 4) {
            printf("| %-3s %s %-16.2f kg |\n", " ", "Weight: ", atof(token));
        }
        token = strtok(NULL, "|");
        tokenIndex++;
    }
    printf("|%-34s|\n", " ");
}

FitnessStatus bmiCategory(User *user) {
    if (user->bmi < 18.5) {
        return UNDERWEIGHT;
    }
    else if (user->bmi < 25) {
        return NORMAL;
    }
    else if (user->bmi < 30) {
        return OVERWEIGHT;
    }
    else {
        return OBESE;
    }
}

char *printStatus(FitnessStatus fitnessStatus) {
    if (fitnessStatus == UNDERWEIGHT) {
        return "Underweight";
    }
    else if (fitnessStatus == NORMAL) {
        return "Normal";
    }
    else if (fitnessStatus == OVERWEIGHT) {
        return "Overweight";
    }
    else if (fitnessStatus == OBESE) {
        return "Obese";
    } 
    else {
        return NULL;
    }
}