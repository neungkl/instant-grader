#ifndef console_h
#define console_h

#include "global.h"
#include <cstdio>

enum FontStyle {
  Green = 1,
  Red = 2,
  White = 4,
  Grey = 8,
  Bold = 16,
  Normal = 32,
  Check = 64,
  Cross = 128
};

void console(const char*, int = (White | Normal));
void consoleln(const char* = "", int = (White | Normal));
void consoleSpliter(char, int = DEFAULT_SPLITTER_WIDTH, int = (White | Normal));

void consoleFile(FILE*);
void consoleHelp();

#endif
