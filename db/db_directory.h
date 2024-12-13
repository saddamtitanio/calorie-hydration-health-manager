#ifndef DB_DIRECTORY_H
#define DB_DIRECTORY_H

char *getDbFilePath();
bool createDbDirectory();
void createDbFile();
void closeDBFile(FILE **file, char **filePath);

#endif