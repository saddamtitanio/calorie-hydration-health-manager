#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <stdbool.h>
#include <time.h>

/* Compatibility with Windows OS and POSIX systems*/
#ifdef _WIN32
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#endif

#define DATABASE_DIR "database"
#define USER_FILE "user.txt"

#define MAX_NAME_LENGTH 70
#define MAX_AGE_DIGIT 3
#define MAX_HEIGHT_DIGIT 3
#define MAX_WEIGHT_DIGIT 3

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH + 1];
    int age;
    float height;
    float weight;
    char datetime[50];
} User;

bool createDbDirectory();
void displayTitleMenu();
char *getDbFilePath();
void createDbFile();
void createUser();
bool isDataValid(int scanf, bool *isValid, const char *msg);
bool rangeCheck(void *value, int dataType, const char *msg, bool *isValid);
int getLastUserId();
void handleProfileOption(int option, User *user);
char *getCurrentLocalDateTime();
void clearInputBuffer();
void getUser(int targetLine, User *user);
void setCurrentUser(User *user);
char *retrieveAllUsers();
void decomposeLine(char *line, User *user);

int main(void) {
    int option;
    User user;

    createDbDirectory();
    createDbFile();
    do {
        displayTitleMenu();
        printf("Option: ");
        scanf("%d", &option);
        clearInputBuffer();
        handleProfileOption(option, &user);

    } while (option != 2 && option != 4);

    return 0;
}

void clearInputBuffer() {
    while(getchar() != '\n');
}

void handleProfileOption(int option, User *user) {
    switch (option) {
        case 1:
            retrieveAllUsers();
            break;
        case 2:
            setCurrentUser(user);
            break;
        case 3:
            createUser();
            break;
        default:
            break;
    }
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
                printf("USER: %s", user->datetime);
                break;
            default:
                break;
        }
        token = strtok(NULL, "|");
        i++;
    }
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

void setCurrentUser(User *user) {
    int userChoice;

    retrieveAllUsers();
    printf("Choose user: ");
    scanf("%d", &userChoice);

    getUser(userChoice, user);
}

void createDbFile() {
    char *filePath = getDbFilePath();
    FILE *file = fopen(filePath, "a");

    if (file == NULL) {
        perror("Error creating database file");
        free(filePath);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    free(filePath);
}

bool rangeCheck(void *value, int dataType, const char *msg, bool *isValid) {
    if (dataType == 0 && *(int *)value <= 0 || dataType == 1 && *(float *)value <= 0.0f) {
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

int getLastUserId() {
    char *filePath = getDbFilePath(USER_FILE);
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

    fclose(file);
    free(filePath);

    return lastId;
}

char *retrieveAllUsers() {
    printf("\n=========== PROFILE LIST ===========\n");

    char *filePath = getDbFilePath(USER_FILE);
    FILE *file = fopen(filePath, "r");

    if (file == NULL) {
        perror("Error opening file");
        free(filePath);
        return NULL; 
    }

    char line[256];
    char *token;
    int tokenIndex = 0;
    int userCount = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        tokenIndex = 0;
        token = strtok(line, "|");
        while (token != NULL) {
            if (tokenIndex == 1) {
                printf("| %-3d %-28s |\n", ++userCount, token);
            }
            token = strtok(NULL, "|");
            tokenIndex++;
        }
    }       

    printf("====================================\n\n");

    return NULL;
}

char *getCurrentLocalDateTime() {
    char *formattedDateTime = malloc(20 * sizeof(char));
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);
    strftime(formattedDateTime, 20, "%Y-%m-%d %H:%M:%S", localTime);
    
    return formattedDateTime;
}

void createUser() {
    printf("\n=========== PROFILE SETUP ===========\n");

    char *filePath = getDbFilePath();
    FILE *file = fopen(filePath, "a+");
    User user;
    char ch;
    bool isValid = true;

    do {
        printf("Name: ");
        fgets(user.name, sizeof(user.name), stdin);
        user.name[strcspn(user.name, "\n")] = '\0';

        if (strlen(user.name) == MAX_NAME_LENGTH) {
            while ((ch = getchar()) != '\n') {
                isValid = false;
            }
            if (!isValid) {
                printf("Name exceeds length limit.\n\n");
                continue;
            }
        }
        
        printf("Age: ");
        if(!isDataValid(scanf("%d", &user.age), &isValid, "Invalid Age.")) continue;
        if(!rangeCheck(&user.age, 0, "Invalid Age.", &isValid)) continue;
        clearInputBuffer();

        printf("Height (cm): ");
        if(!isDataValid(scanf("%f", &user.height), &isValid, "Invalid Height.")) continue;
        if(!rangeCheck(&user.height, 1, "Invalid Height.", &isValid)) continue;
        clearInputBuffer();

        printf("Weight (kg): ");
        if(!isDataValid(scanf("%f", &user.weight), &isValid, "Invalid Weight.")) continue;
        if(!rangeCheck(&user.weight, 1, "Invalid Weight.", &isValid)) continue;
        clearInputBuffer();

        isValid = true;
        
    } while (!isValid);
    
    char *currDateTime = getCurrentLocalDateTime();
    int bufferLength = strlen(user.name) + MAX_AGE_DIGIT + MAX_HEIGHT_DIGIT + MAX_WEIGHT_DIGIT + 10 + strlen(currDateTime) + 4;
    char *buffer = malloc(bufferLength);

    if (!buffer) {
        perror("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
    user.id = getLastUserId() + 1;
    
    snprintf(buffer, bufferLength, "%d|%s|%d|%.2f|%.2f|%s\n", user.id, user.name, user.age, user.height, user.weight, currDateTime);
    fprintf(file, buffer);
    
    free(filePath);
    free(buffer);

    fclose(file);

    printf("\n\n");
}

char *getDbFilePath() {
    int filePathSize = strlen(DATABASE_DIR) + strlen("/") + strlen(USER_FILE);
    char *filePath = malloc(filePathSize + 1);
    
    if (filePath == NULL) {
        exit(EXIT_FAILURE);
    }    
    snprintf(filePath, filePathSize + 1, "%s/%s", DATABASE_DIR, USER_FILE);

    return filePath;
}

bool createDbDirectory() {
    const char *folder = DATABASE_DIR;
    struct stat sb;

    if (stat(folder, &sb) == -1) {
        if (mkdir(DATABASE_DIR, 0777) == 0) {
            return true;
        } else {
            perror("Error creating directory.");
            exit(EXIT_FAILURE);
        }
    }
    return true;
}

void displayTitleMenu() {
    printf("*************************************************\n");
    printf("*                                               *\n");
    printf("*          CALORIE & HYDRATION MANAGER          *\n");
    printf("*                                               *\n");
    printf("*************************************************\n");
    printf("1. View Profiles\t\t3. Add Profile\n");
    printf("2. Choose Profile\t\t4. Exit\n");
}