#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils/menu.h"
#include "utils/utils.h"
#include "db/db_directory.h"
#include "controller/usersController.h"
#include "controller/healthController.h"
#include "utils/glass.h"

void handleProfileOption(int option, User *user);
void healthProfile(User *user);
void displayHealthProfile(User *user, Progress progress, UserHealthLog *healthLog);
void handleLogHealth(int option, UserHealthLog *healthLog, User *user, Progress progress);

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
    Progress progress;
    UserHealthLog healthLog = {.userID = -1, .currentCalorieIntake = 0, .currentWaterIntake = 0, .date = ""};
    int option;
    float targetWeight;
    int noOfDays, lifestyleOption;
    bool isValid;
    
    printf("\n");
    do {
        if (!isNullHealthProfile(user->id)) {
            progress = setHealthProfile(user->id);
            healthLog.userID = user->id;
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
        displayHealthProfile(user, progress, &healthLog);
        printf("Option: ");
        scanf("%d", &option);
        handleLogHealth(option, &healthLog, user, progress);

    } while (option != 4);
}

void handleLogHealth(int option, UserHealthLog *healthLog, User *user, Progress progress) {
    FitnessStatus fitnessStatus = bmiCategory(user->bmi);
    float BMR = calculateBMR(user, progress);
    char exit;

    switch (option) {
        case 1: {
            float waterIntake;
            printf("Enter the amount of water intake (in ml): ");
            scanf("%f", &waterIntake);
            healthLog->currentWaterIntake += waterIntake;
            clearInputBuffer();
            printf("You have logged %.2f ml of water.\n", waterIntake);
            break;
        }
        case 2: {
            float calorieIntake;
            printf("Enter the amount of calorie intake: ");
            scanf("%f", &calorieIntake);
            healthLog->currentCalorieIntake += calorieIntake;
            clearInputBuffer();
            printf("You have logged %.2f calories.\n", calorieIntake);
            break;
        }
        case 3:
            do {
                mealComposition(BMR, fitnessStatus);
                printf("Exit? (Y/n): ");
                scanf(" %c", &exit);
            } while (exit != 'Y' && exit != 'y');
            break;
        case 4:
            break;
        default:
            printf("Invalid option. Please try again.\n");
            break;
    }
}

void displayHealthProfile(User *user, Progress progress, UserHealthLog *healthLog) {
    FitnessStatus fitnessStatus = bmiCategory(user->bmi);
    float BMR = calculateBMR(user, progress);
    char *date = getDate();
    float goalWater = targetWaterIntake(user->weight);

    float caloriePercentage = calculateCaloriePercentage(BMR, healthLog->currentCalorieIntake);
    float waterPercentage = calculateWaterPercentage(goalWater, healthLog->currentWaterIntake);
    
    printf("\n ================= HEALTH PROFILE ==================\n");
    printf("|                                                   |\n");
    printf("| Current Weight: %-30.2f kg |\n", user->weight);
    printf("| Your BMI is: %-36.2f |\n", user->bmi);
    printf("| BMI Category: %-35s |\n", printStatus(fitnessStatus));
    printf("| Target Weight: %-31.2f kg |\n", progress.targetWeight);
    printf("| Daily Basal Metabolic Rate (BMR): %-11.2f cal |\n", BMR); 
    printf("| Daily Water Intake Goal: %-21.2f ml |\n", goalWater);
    printf("|                                                   |\n");
    printf(" =============== PROGRESS (%s) ===============\n", date);
    printf("|                                                   |\n");
    printf("| Water Intake: %-25.2f ml (%2.f %%) |\n", healthLog->currentWaterIntake, waterPercentage);
    printf("| Calorie Intake: %-22.2f cal (%2.f %%) |\n", healthLog->currentCalorieIntake, caloriePercentage);
    printf("|                                                   |\n");
    printf(" ===================================================\n");
    
    createGlassArt(waterPercentage);

    printf("\n1. Log water intake\t\t2. Log calorie intake\n3. View nutrient composition\t");
    printf("4. Exit\n");
    free(date);
}