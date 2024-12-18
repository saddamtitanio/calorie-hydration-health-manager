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

FitnessStatus bmiCategory(float bmi) {
    if (bmi < 18.5) {
        return UNDERWEIGHT;
    }
    else if (bmi < 25) {
        return NORMAL;
    }
    else if (bmi < 30) {
        return OVERWEIGHT;
    }
    else {
        return OBESE;
    }
}

bool isValidWeightTarget(float targetWeight, float height) {
    float newBMI = calculateBMI(height, targetWeight);
    FitnessStatus fitnessStatus = bmiCategory(newBMI);
    height /= 100;

    if (fitnessStatus == UNDERWEIGHT || fitnessStatus == OVERWEIGHT || fitnessStatus == OBESE) {
        float lowWeightRange = NORMAL_LOW_THRESHOLD_VALUE * height * height;
        float highWeightRange = NORMAL_HIGH_THRESHOLD_VALUE * height * height;
        printf("Invalid weight range. Valid range (%.2f kg - %.2f kg)\n", lowWeightRange, highWeightRange);
        return false;
    }
    return true;
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

float calculateWaterPercentage(float targetWaterIntake, float currentWaterIntake) {
    if (targetWaterIntake == 0) {
        return 0;
    }
    return (currentWaterIntake / targetWaterIntake) * 100;
}

float calculateCaloriePercentage(float targetCalorie, float currentCalorie) {
    if (targetCalorie == 0) {
        return 0;
    }
    return (currentCalorie / targetCalorie) * 100;
}

float targetWaterIntake(float weight) {
    return weight * 0.03 * 1000;
}

void mealComposition(float BMR, FitnessStatus BMICategory) {
    float protein = 1;
    float fat = 1;
    float carb = 1;
    
    if (BMICategory == UNDERWEIGHT) {
        protein = 0.35;
        fat = 0.20;
        carb = 0.45;
    }
    else if (BMICategory == NORMAL) {
        protein = 0.35;
        fat = 0.20;
        carb = 0.45;
    }
    else if (BMICategory == OVERWEIGHT || BMICategory == OBESE) {
        protein = 0.5;
        carb = 0.3;
        fat = 0.2;
    }

    printf("\nNutritional Breakdown:\n");
    printf("--------------------------------\n");
    printf("| %-15s | %-10s |\n", "Nutrient", "Amount (g)");
    printf("--------------------------------\n");
    printf("| %-15s | %10.2f |\n", "Protein", BMR * protein);
    printf("| %-15s | %10.2f |\n", "Carbohydrates", BMR * carb);
    printf("| %-15s | %10.2f |\n", "Fat", BMR * fat);
    printf("--------------------------------\n");

}

char *getDate() {
    int bufferCapacity = 80;
    char *buffer = malloc(bufferCapacity);

    time_t now;
    struct tm *tm_struct;

    time(&now);
    tm_struct = localtime(&now);

    strftime(buffer, bufferCapacity, "%D", tm_struct);

    return buffer;
}