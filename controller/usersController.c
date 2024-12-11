#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/utils.h"
#include "../db/db_directory.h"
#include "usersController.h"

int getLastUserId() {
    char *filePath = getDbFilePath();
    FILE *file = fopen(filePath, "r");
    char line[256];
    int lastId = 0;

    if (file == NULL) {
        perror("Error opening file");
        free(filePath);
        return -1; 
    }

    while (fgets(line, sizeof(line), file) != NULL) {
        char *token = strtok(line, "|");
        if (token != NULL) {
            sscanf(token, "%d", &lastId);
        }
    }

    fclose(file);
    free(filePath);

    return lastId;
}

void createUser(User *user) {
    printf("\n=========== PROFILE SETUP ===========\n");

    char *filePath = getDbFilePath();
    FILE *file = fopen(filePath, "a+");
    char ch;
    bool isValid = true;

    do {
        printf("Name: ");
        fgets(user->name, sizeof(user->name), stdin);
        user->name[strcspn(user->name, "\n")] = '\0';

        if (strlen(user->name) == 0) {
            isValid = false;
            printf("Name cannot be an empty string.\n\n");
            continue;
        }
        if (strlen(user->name) == MAX_NAME_LENGTH) {
            while ((ch = getchar()) != '\n') {
                isValid = false;
            }
            if (!isValid) {
                printf("Name exceeds length limit.\n\n");
                continue;
            }
        }
        
        printf("Age: ");
        if(!isDataValid(scanf("%d", &user->age), &isValid, "Invalid Age.")) continue;
        if(!rangeCheck(&user->age, 0, "Invalid Age.", &isValid)) continue;
        clearInputBuffer();

        printf("Height (cm): ");
        if(!isDataValid(scanf("%f", &user->height), &isValid, "Invalid Height.")) continue;
        if(!rangeCheck(&user->height, 1, "Invalid Height.", &isValid)) continue;
        clearInputBuffer();

        printf("Weight (kg): ");
        if(!isDataValid(scanf("%f", &user->weight), &isValid, "Invalid Weight.")) continue;
        if(!rangeCheck(&user->weight, 1, "Invalid Weight.", &isValid)) continue;
        clearInputBuffer();

        isValid = true;
        
    } while (!isValid);
    
    char *currDateTime = getCurrentLocalDateTime();
    int bufferLength = strlen(user->name) + MAX_AGE_DIGIT + MAX_FLOAT_DIGIT * 3 + strlen(currDateTime) + MEMBERS_COUNT - 1;
    char *buffer = malloc(bufferLength);

    if (!buffer) {
        perror("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }

    user->id = getLastUserId() + 1;
    user->bmi = calculateBMI(user->height, user->weight);

    snprintf(buffer, bufferLength, "%d|%s|%d|%.2f|%.2f|%.2f|%s\n", user->id, user->name, user->age, user->height, user->weight, user->bmi, currDateTime);
    fprintf(file, buffer);
    
    free(filePath);
    free(buffer);

    fclose(file);

    printf("User sucessfully created.\n");
    printf("\n\n");
}

void getUser(int targetLine, User *user) {
    char *filePath = getDbFilePath();
    FILE *file = fopen(filePath, "r");
    int currentLine = 1;
    char buffer[255];

    if (file == NULL) {
        perror("Error opening file.");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (currentLine == targetLine) {
            decomposeLine(buffer, user);
            break;
        }
        currentLine++;
    }

    if (currentLine < targetLine) {
        printf("User does not exist.\n");
    }

    fclose(file);
}


int displayUsers() {
    int userCount = 0;
    char line[256];
    char *token;
    int tokenIndex = 0;
    char *filePath = getDbFilePath();
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        perror("Error opening file");
        free(filePath);
        return -1; 
    }
    printf("\n=========== PROFILE LIST ===========\n");
    while (fgets(line, sizeof(line), file) != NULL) {
        tokenIndex = 0;
        token = strtok(line, "|");
        while (token != NULL) {
            if (tokenIndex == 1) {
                printf("| %-3d %-28s |\n", ++(userCount), token);
            }
            token = strtok(NULL, "|");
            tokenIndex++;
        }
    }
    printf("=====================================\n\n");   
    fclose(file);
    free(filePath);
    
    return userCount;
}

void retrieveAllUsers() {
    char option;
    char line[256];
    char *filePath = getDbFilePath();
    FILE *file = fopen(filePath, "r");

    int userCount = displayUsers();

    printf("Show details? (Y/n): ");
    scanf("%c", &option);

    if (option == 'Y' || option == 'y') {
        int noOfPages = ceil((double)userCount / PAGE_BREAK_LIMIT);
        int currentUserCount = 0;
        int page = 1;

        while (true) {
            printf("\n=========== PAGE %d/%d ===============\n", page, noOfPages);
            fseek(file, 0, SEEK_SET);
            currentUserCount = (page - 1) * PAGE_BREAK_LIMIT;

            for (int i = 0; i < currentUserCount; i++) {
                fgets(line, sizeof(line), file);
            }

            for (int i = 0; i < PAGE_BREAK_LIMIT && fgets(line, sizeof(line), file) != NULL; i++) {
                showDetails(line, &currentUserCount);
            }

            clearInputBuffer();
            printf("====================================\n");
            printf("See other pages? (Y/n): ");
            scanf("%c", &option);

            if (option == 'Y' || option == 'y') {
                printf("Select page (1-%d): ", noOfPages);
                scanf("%d", &page);

                if (page < 1 || page > noOfPages) {
                    printf("Invalid page. Returning to main menu.\n");
                    break;
                }
            } else {
                break;
            }
        }
    }

    printf("\n");
    fclose(file);
    free(filePath);
}

void decomposeLine(char *line, User *user) {
    char *token;
    token = strtok(line, "|");
    
    int i = 0;
    while (token != NULL) {
        switch (i) {
            case 0:
                user->id = atoi(token);
                break;
            case 1:
                strcpy(user->name, token);
                user->name[strlen(user->name)] = '\0';
                break;
            case 2:
                user->age = atoi(token);
                break;
            case 3:
                user->height = atof(token);
                break;
            case 4:
                user->weight = atof(token);
                break;
            case 5:
                strcpy(user->datetime, token);
                user->datetime[strlen(user->datetime)] = '\0';
                break;
            default:
                break;
        }
        token = strtok(NULL, "|");
        i++;
    }
}

void setCurrentUser(User *user) {
    int userChoice;

    displayUsers();
    printf("Choose user: ");
    scanf("%d", &userChoice);

    getUser(userChoice, user);
}

// void deleteUser() {
//     char *filePath = getDbFilePath();
//     FILE *file = fopen(filePath, "r");
// }