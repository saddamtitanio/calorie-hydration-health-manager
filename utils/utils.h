#ifndef UTILS_H
#define UTILS_H

#define NORMAL_LOW_THRESHOLD_VALUE 18.5
#define NORMAL_HIGH_THRESHOLD_VALUE 24.9

#include "../controller/usersController.h"
#include "../controller/healthController.h"

void clearInputBuffer();
char *getCurrentLocalDateTime();
bool rangeCheck(void *value, int dataType, const char *msg, bool *isValid);
bool isDataValid(int scanf, bool *isValid, const char *msg);
float calculateBMI(float height, float weight);
void showDetails(char *line, int *currentUserCount);
FitnessStatus bmiCategory(float bmi);
char *printStatus(FitnessStatus fitnessStatus);
bool isValidWeightTarget(float targetWeight, float height);

#endif