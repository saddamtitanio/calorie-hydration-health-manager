#ifndef HEALTH_CONTROLLER_H
#define HEALTH_CONTROLLER_H

#include "usersController.h"

void writeHealthFile(int userId, float targetWeight, int targetDays, Lifestyle lifestyle);
bool isNullHealthProfile(int id);
char *readHealthProfile(int id);
Progress setHealthProfile(int id);
void activityLevelMultiplier(float *BMR, Lifestyle lifestyle);
float calculateBMR(User *user, Progress progress);

#endif