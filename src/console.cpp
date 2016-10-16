#include <cstdio>
#include "console.h"

void console(char* word, int style) {

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

  printf("%s", word);
  printf("\e[0m");
}
