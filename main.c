#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "utils/menu.h"
#include "utils/utils.h"
#include "db/db_directory.h"
#include "controller/usersController.h"

void handleProfileOption(int option, User *user);

int main(void) {
    int option;
    User user = {.name = ""};
    
    createDbFile();
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
        if (option == 6) return 0;

        clearInputBuffer();
        handleProfileOption(option, &user);
    } while (option != 6 && option != 5);

    printf("\n");
    if (option == 5) {
        printf("TEST.\n");
    }

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
            break;
        case 5:
            break;
        default:
            break;
    }
}