#ifndef util_h
#define util_h

#include <cstdio>

bool isHeadEqual(char*, char*);

char* strReplace(char*, const char*, const char*);
char* toString(int);

FILE* createFileHandler(char*, char*);

#endif
