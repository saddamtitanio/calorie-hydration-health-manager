#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#include "utils/menu.h"
#include "utils/utils.h"
#include "db/db_directory.h"
#include "controller/usersController.h"
#include "controller/healthController.h"


void handleProfileOption(int option, User *user);
void healthProfile(User *user);
void displayHealthProfile(User *user, Progress progress);


int main(void) {
    int option;
    User user = {.name = ""};
   
    createDbFile(USER_FILE);
    do {
        displayTitleMenu();
        if (strcmp(user.name, "") == 0) printf("Active Profile: None\n");
        else printf("Active Profile: %s\n", user.name);
        displayOptionMenu();


        printf("Option: ");
        if (!scanf("%d", &option) || (option < 1 || option > 6)) {
            printf("Invalid option.\n\n");
            clearInputBuffer();
            continue;
        }
        if (option == 5 && strcmp(user.name, "") == 0) {
            printf("Select a user continue.\n\n");
            option = -1;
            continue;
        }
        if (option == 6) return 0;


        clearInputBuffer();
        handleProfileOption(option, &user);
    } while (option != 6 && option != 5);


    return 0;
}


void handleProfileOption(int option, User *user) {
    switch (option) {
        case 1:
            retrieveAllUsers();
            break;
        case 2:
            createUser(user);
            break;
        case 3:
            setCurrentUser(user);
            break;
        case 4:
            deleteUser();
            break;
        case 5:
            healthProfile(user);
            break;
        default:
            break;
    }
}


void healthProfile(User *user) {
    FitnessStatus fitnessStatus = bmiCategory(user->bmi);
    Progress progress;
    int option;
    float targetWeight;
    int noOfDays, lifestyleOption;
    bool isValid;
   
    printf("\n");
    do {
        if (!isNullHealthProfile(user->id)) {
            progress = setHealthProfile(user->id);
        }
        else {
            printf("Health profile not yet created.\n");
            do {
                printf("\nTarget Weight (kg): ");
                scanf("%f", &targetWeight);
                isValid = isValidWeightTarget(targetWeight, user->height);
            } while (!isValid);
            printf("Number of days to reach the goal: ");
            scanf("%d", &noOfDays);


            printf("Select Lifestyle\n1. Sedentary\n2. Lightly active\n3. Moderately active\n");
            printf("4. Very active\n5. Extra active\nOption (1 - 5): ");
            scanf("%d", &lifestyleOption);
            writeHealthFile(user->id, targetWeight, noOfDays, lifestyleOption);
            progress = setHealthProfile(user->id);
        }
        displayHealthProfile(user, progress);
        scanf("%d", &option);
    } while (option != -1);
}


void displayHealthProfile(User *user, Progress progress) {
    FitnessStatus fitnessStatus = bmiCategory(user->bmi);
    printf("\n=========== HEALTH PROFILE ===========\n");
    printf("| Current weight: %-15.2f kg |\n", user->weight);
    printf("| Your BMI is: %-21.2f |\n", user->bmi);
    printf("| BMI Category: %-20s |\n", printStatus(fitnessStatus));
    printf("| Target weight: %-16.2f kg |\n", progress.targetWeight);
    printf("======================================\n");


    float BMR = calculateBMR(user, progress);
}
