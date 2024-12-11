#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define CAP_HEIGHT 3
#define SLANT_HEIGHT 4
#define BODY_HEIGHT 6
#define CAP_LENGTH 5
#define BODY_LENGTH 15

// Random character generator
char randomizeSpecialChar() {
    const char *specialChars = "!@#$%^&*()+-<>?";
    int numSpecialChars = strlen(specialChars);
    return specialChars[rand() % numSpecialChars];
}

void printSpaces(int count) {
    for (int i = 0; i < count; i++) {
        printf(" ");
    }
}

// Top of the bottle
void printTop() {
    printSpaces(11);
    printf("_____\n");
}

// Cap of the bottle
void printCap(int *bottleHeight, int heightFilled) {
    for (int i = 0; i < CAP_HEIGHT; i++) {
        printSpaces(10);
        printf("|");
        if (*bottleHeight == heightFilled + 1) {
            for (int j = 0; j < CAP_LENGTH; j++) printf("~");
        } else if (*bottleHeight <= heightFilled) {
            for (int j = 0; j < CAP_LENGTH; j++) printf("%c", randomizeSpecialChar());
        } else {
            for (int j = 0; j < CAP_LENGTH; j++) printf(" ");
        }
        printf("|\n");
        (*bottleHeight)--;
    }
}

// Slanted neck of the bottle
void printSlantedNeck(int *bottleHeight, int heightFilled) {
    for (int i = 1; i <= SLANT_HEIGHT; i++) {
        printSpaces(10 - i);
        printf("/");
        if (*bottleHeight == heightFilled + 1) {
            for (int j = 0; j < 2 * i + 5; j++) printf("~");
        } else if (*bottleHeight <= heightFilled) {
            for (int j = 0; j < 2 * i + 5; j++) printf("%c", randomizeSpecialChar());
        } else {
            for (int j = 0; j < 2 * i + 5; j++) printf(" ");
        }
        printf("\\\n");
        (*bottleHeight)--;
    }
}

// Main body of the bottle
void printBody(int *bottleHeight, int heightFilled) {
    for (int i = 0; i < BODY_HEIGHT; i++) {
        printSpaces(5);
        printf("|");
        if (*bottleHeight == heightFilled + 1 && heightFilled != 0) {
            for (int j = 0; j < BODY_LENGTH; j++) printf("~");
        } else if (*bottleHeight <= heightFilled) {
            for (int j = 0; j < BODY_LENGTH; j++) printf("%c", randomizeSpecialChar());
        } else {
            for (int j = 0; j < BODY_LENGTH; j++) printf(" ");
        }
        printf("|\n");
        (*bottleHeight)--;
    }
}

// Bottom of the bottle
void printBottom() {
    printSpaces(5);
    printf("|_______________|\n");
}

// Main function to create glass art
void createGlassArt(float percentage) {
    srand(time(NULL));
    int bottleHeight = CAP_HEIGHT + SLANT_HEIGHT + BODY_HEIGHT;
    int heightFilled = round((double)percentage / ceil((double)100.0 / (double)bottleHeight));

    printTop();
    printCap(&bottleHeight, heightFilled);
    printSlantedNeck(&bottleHeight, heightFilled);
    printBody(&bottleHeight, heightFilled);
    printBottom();
}