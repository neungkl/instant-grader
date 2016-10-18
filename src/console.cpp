#include <cstdio>
#include <cstring>
#include "console.h"
#include "global.h"

/*
  Console output to terminal screen
  @param word string to output
  @param style style of font etc. color, bold
*/
void console(const char* word, int style) {

  if(style & Green) {
    printf("\e[32m");
  } else if(style & Red) {
    printf("\e[31m");
  } else if(style & White) {
    printf("\e[39m");
  } else if(style & Grey) {
    printf("\e[30m");
  }

  if(style & Bold) {
    printf("\e[1m");
  }

  if(style & Check) {
    printf("\xE2\x9C\x94");
  } else if(style & Cross) {
    printf("\xE2\x9C\x98");
  }

  printf("%s", word);
  printf("\e[0m");
}

void consoleln(const char* word, int style) {
  console(word, style);
  printf("\n");
}

void consoleSpliter(char sr, int len, int style) {
  char srl[len+1];
  for(int i=0; i<len; i++) srl[i] = sr;
  srl[len] = '\0';

  consoleln(srl, style);
}

/*
  Console all content from file
  @param file File to print
*/
void consoleFile(FILE* file) {

  char buffer[BUFFER_SIZE + 1];
  int len;

  buffer[BUFFER_SIZE] = '\0';

  rewind(file);
  while(fgets(buffer, BUFFER_SIZE, file) != NULL) {
    len = strlen(buffer); --len;
    if(buffer[len] == '\n') buffer[len--] = '\0';
    if(buffer[len] == '\r') buffer[len--] = '\0';
    puts(buffer);
  }

}
