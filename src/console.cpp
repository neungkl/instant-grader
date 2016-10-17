#include <cstdio>
#include "console.h"

void console(const char* word, int style) {

  if(style & Green) {
    printf("\e[32m");
  } else if(style & Red) {
    printf("\e[31m");
  } else if(style & White) {
    printf("\e[39m");
  }

  if(style & Bold) {
    printf("\e[1m");
  }

  if(style & Check) {
    printf("✔");
  } else if(style & Cross) {
    printf("✘");
  }

  printf("%s", word);
  printf("\e[0m");
}

void consoleln(const char* word, int style) {
  console(word, style);
  printf("\n");
}

void consoleSpliter(char sr) {
  for(int i=0; i<50; i++) printf("%c", sr);
  printf("\n");
}
