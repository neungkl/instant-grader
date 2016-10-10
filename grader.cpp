#include <cstdio>

enum ConsoleColor {
  Green, Red, White
};

enum ConsoleFontStyle {
  Bold, Normal
};

void console(const char* word, ConsoleColor csCol = White, ConsoleFontStyle fntStyle = Normal) {
  switch(csCol) {
    case Green  : printf("\e[32m"); break;
    case Red    : printf("\e[31m"); break;
    case White  : printf("\e[39m"); break;
  }
  switch(fntStyle) {
    case Bold   : printf("\e[1m"); break;
  }

  printf("%s", word);
  printf("\e[0m");
}

int main(int argc, char** argv) {

  if(argc == 1) {
    console("Please enter specific file to run test.\n", Red, Bold);
    console("For Example :\n", White);
    console("  ./grader add.cpp", White, Bold);
    console(" (if you have available test in same file)\n", White);
    console("  ./grader add.cpp test_case.txt\n", White, Bold);
  } else if(argc == 2) {

  } else if(argc == 3) {

  } else {

  }

  return 0;
}
