#ifndef UTILS_H
#define UTILS_H

void clearInputBuffer();
char *getCurrentLocalDateTime();
bool rangeCheck(void *value, int dataType, const char *msg, bool *isValid);
bool isDataValid(int scanf, bool *isValid, const char *msg);
float calculateBMI(float height, float weight);
void showDetails(char *line, int *currentUserCount);

#endif