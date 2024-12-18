#ifndef USERS_CONTROLLER_H
#define USERS_CONTROLLER_H

#define MAX_NAME_LENGTH 70
#define MAX_AGE_DIGIT 4
#define MAX_FLOAT_DIGIT 7
#define MEMBERS_COUNT 10
#define PAGE_BREAK_LIMIT 5

#define TEMP_DB_FILE "database/temp.txt"

typedef enum {
    SEDENTARY,
    LIGHTLY_ACTIVE,
    MODERATELY_ACTIVE,
    VERY_ACTIVE,
    EXTRA_ACTIVE
} Lifestyle;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH + 1];
    int age;
    float height;
    float weight;
    char datetime[50];
    float bmi;
    int sex;
} User;

typedef struct {
    int userId;
    float targetWeight;
    int targetDays;
    Lifestyle lifestyle;
} Progress;

typedef struct {
    int userID;
    float currentWaterIntake; 
    float currentCalorieIntake;
    char date[50];
} UserHealthLog;

typedef enum {
    UNDERWEIGHT,
    NORMAL,
    OVERWEIGHT,
    OBESE
} FitnessStatus;


void setCurrentUser(User *user);
int getLastUserId();
void createUser(User *user);
char *getUser(int targetLine, User *user);
int displayUsers();
void retrieveAllUsers();
int deleteUser();

#endif