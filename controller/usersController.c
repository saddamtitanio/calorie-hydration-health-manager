#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/utils.h"
#include "../db/db_directory.h"
#include "usersController.h"

int getLastUserId() {
    char *filePath = getDbFilePath(USER_FILE);
    if (filePath == NULL) {
        perror("Failed to get path.");
        return -1;
    }

    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        perror("Error opening file");
        free(filePath);
        return -1; 
    }

    char line[256];
    int lastId = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        char *token = strtok(line, "|");
        if (token != NULL) {
            sscanf(token, "%d", &lastId);
        }
    }

    closeDBFile(&file, &filePath);

    return lastId;
}

void createUser(User *user) {
    printf("\n=========== PROFILE SETUP ===========\n");

    char *filePath = getDbFilePath(USER_FILE);

    if (filePath == NULL) {
        perror("Failed to get path.");
        return;
    }
    FILE *file = fopen(filePath, "a+");

    if (file == NULL) {
        perror("Error opening file");
        free(filePath);
        return; 
    }
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

        printf("Sex\n1. Male\n2. Female\nOption: ");
        if(!isDataValid(scanf("%d", &user->sex), &isValid, "Invalid Sex.")) continue;
        if (user->sex != 1 && user->sex != 2) {
            isValid = false;
            printf("Invalid Sex.\n\n");
            clearInputBuffer();
            continue;
        }
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
    int bufferLength = strlen(user->name) + MAX_AGE_DIGIT + MAX_FLOAT_DIGIT * 3 + strlen(currDateTime) + 2 + MEMBERS_COUNT;
    char *buffer = malloc(bufferLength);

    if (!buffer) {
        perror("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }

    user->id = getLastUserId() + 1;
    user->bmi = calculateBMI(user->height, user->weight);

    snprintf(buffer, bufferLength, "%d|%s|%d|%.2f|%.2f|%.2f|%d|%s\n", user->id, user->name, user->age, user->height, user->weight, user->bmi, user->sex, currDateTime);
    fprintf(file, buffer);
    
    free(buffer);

    closeDBFile(&file, &filePath);

    printf("User sucessfully created.\n");
    printf("\n\n");
}

char *getUser(int targetLine, User *user) {
    char *filePath = getDbFilePath(USER_FILE);
    if (filePath == NULL) {
        perror("Failed to get path.");
        return NULL;
    }

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file.");
        return NULL;
    }

    int currentLine = 1;
    char *buffer = malloc(256);

    if (buffer == NULL) {
        perror("Error memory allocation.");
        return NULL;
    }

    while (fgets(buffer, 256, file) != NULL) {
        if (currentLine == targetLine) {
            closeDBFile(&file, &filePath);
            return buffer;
        }
        currentLine++;
    }

    printf("User does not exist.\n");

    closeDBFile(&file, &filePath);
    return NULL;
}


int displayUsers() {
    int userCount = 0;
    char line[256];
    char *token;
    int tokenIndex = 0;

    char *filePath = getDbFilePath(USER_FILE);
    if (filePath == NULL) {
        perror("Failed to get path.");
        return -1;
    }

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        free(filePath);
        return -1; 
    }

    printf("\n=========== PROFILE LIST ===========\n");

    if (fgetc(file) == EOF) {
        printf("\tNo profiles created.\n");
        printf("=====================================\n\n");
        free(filePath); 
        fclose(file);
        return -1;
    }

    fseek(file, 0, SEEK_SET);

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
    printf("====================================\n\n");  

    closeDBFile(&file, &filePath); 
    
    return userCount;
}

void retrieveAllUsers() {
    char option[3];
    char line[256];
    char *filePath = getDbFilePath(USER_FILE);
    
    if (filePath == NULL) {
        perror("Failed to get path.");
        return;
    }

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error opening file");
        free(filePath);
        return; 
    }

    int userCount = displayUsers();

    if (userCount > 0) {
        printf("Show details? (Y/n): ");
        fgets(option, sizeof(option), stdin);
        option[strlen(option)] = '\0';
        option[strcspn(option, "\n")] = 0;
        
        if (strlen(option) > 1) {
            clearInputBuffer();
            printf("Invalid option.\n\n");
            return;
        }
    }

    if (option[0] == 'Y' || option[0] == 'y') {
        int noOfPages = ceil((double)userCount / PAGE_BREAK_LIMIT);
        int currentUserCount = 0;
        int page = 1;
        option[0] = '\0';

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

            printf("====================================\n");

            if (noOfPages > 1) {
                printf("See other pages? (Y/n): ");
                fgets(option, sizeof(option), stdin);
                option[strlen(option)] = '\0';
                option[strcspn(option, "\n")] = 0;
                if (strlen(option) > 1) {
                    printf("Invalid option.\n\n");
                    clearInputBuffer();
                    break;
                }
            }
            else {
                break;
            }

            if (option[0] == 'Y' || option[0] == 'y') {
                printf("Select page (1-%d): ", noOfPages);
                
                if ((!scanf("%d", &page)) || (page < 1 || page > noOfPages)) {
                    printf("Invalid page. Returning to main menu.\n");
                    clearInputBuffer();
                    break;
                }
                clearInputBuffer();
            } else {
                break;
            }
        }
    }
    else {
        if (option[0] != 'n' && option[0] != 'N') {
            printf("Invalid input.\n\n");
        }
    }

    printf("\n");

    closeDBFile(&file, &filePath);
}


void setCurrentUser(User *user) {
    int userChoice;

    int canChoice=displayUsers();
    if (canChoice != -1) {
        printf("Choose user: ");
        scanf("%d", &userChoice);
    }
    else return;
    char *token;
    token = strtok(getUser(userChoice, user), "|");
    
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
                user->bmi = atof(token);
                break;
            case 6:
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

int deleteUser() {
    char *filePath = getDbFilePath(USER_FILE);
    if (filePath == NULL) {
        perror("Failed to get path.");
        return -1;
    }

    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        perror("File cannot be opened.");
        free(filePath);
        return -1;
    }

    FILE *tempFile = fopen("database/temp.txt", "w");
    if (tempFile == NULL) {
        perror("Error opening temp file.");
        free(filePath);
        return -1;
    }
    
    int selectedLine;
    int userCount = displayUsers();
    int ch = fgetc(file);

    if (ch != EOF) {
        printf("User to be deleted (-1 to exit): ");
        if (!scanf("%d", &selectedLine) || (selectedLine < 1 || selectedLine > userCount)) {
            printf("Invalid input. Returning to main menu.\n\n");
            clearInputBuffer();
            closeDBFile(&file, &filePath);
            return -1;
        }
    }
    else {
        printf("No profiles created.\n\n");
        return -1;
    }

    int bufferSize = 256;
    char *buffer = malloc(bufferSize);
    fseek(file, 0, SEEK_SET);
    int lineCount = 0;
    int bufferCount = 0;

    while ((ch = fgetc(file)) != EOF) {
       if (ch == '\n') {
            lineCount++;
        }
        if (lineCount == selectedLine - 1) {
            continue;
        }
        if (bufferCount == bufferSize - 1) {
            char *temp = realloc(buffer, bufferSize * 2);
            if (temp == NULL) {
                perror("Memory reallocation failed.");
                free(buffer);
                return -1;
            }
            buffer = temp;
            bufferSize *= 2;
        }
        buffer[bufferCount++] = ch;
    }
    buffer[bufferCount] = '\0';
    fprintf(tempFile, buffer);

    fclose(tempFile);
    fclose(file);

    if (remove(filePath) != 0) {
        perror("Error deleting the original file");
        return -1;
    }

    if (rename(TEMP_DB_FILE, filePath) != 0) {
        perror("Error renaming temporary file");
    } else {
        printf("User successfully deleted.\n\n");
    }

    free(buffer);
    free(filePath);

    return 0;
}