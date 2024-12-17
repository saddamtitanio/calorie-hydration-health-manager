#ifndef DB_DIRECTORY_H
#define DB_DIRECTORY_H

#define DATABASE_DIR "database"
#define USER_FILE "user.txt"
#define HEALTH_FILE "health.txt"

char *getDbFilePath(const char *fileName);
bool createDbDirectory();
void createDbFile(const char *fileName);
void closeDBFile(FILE **file, char **filePath);

#endif