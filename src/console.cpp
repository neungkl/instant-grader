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

/*
  Console help file
*/
void consoleHelp() {
  console  ("Usage: ", Bold); consoleln("./grader [OPTIONS] YOUR_FILE");
  consoleln("       ./grader YOUR_FILE [OPTIONS]");

  consoleln();
  consoleln("Options:", Bold);
  consoleln("  -v             Show content in both accepted and rejected case");
  consoleln("  -o             Specific execute file path");
  consoleln("  -b             Specific bin path");
  consoleln("  -in STR        Input delimiter in script file (default: \"===\")");
  consoleln("  -out STR       Output delimiter in script file (default: \"---\")");
  consoleln("  --compile CMD  Custom compile command");
  consoleln("                 default: \"gcc -o %prog %file\"");
  consoleln("  --run CMD      Custom run command");
  consoleln("                 default: \"%prog < %in > %out\"");

  consoleln();
  consoleln("Options Pattern:", Bold);
  consoleln("  Some specific options are allow to enter string like %str");
  consoleln("  to define some specific context while running the grader");

  consoleln();
  consoleln("  %bin     Bin directory");
  consoleln("  %code    Your code file");
  consoleln("  %prog    Compiled program (for interpreted language this file not created)");
  consoleln("  %progx   Compiled program name (only a name not include directory path)");
  consoleln("  %in      Input test case file");
  consoleln("  %out     Output file after run case");

  consoleln();
  consoleln("  Some options are not supported some pattern.");
  consoleln("  Here a lookup table for available option pattern");

  consoleln();
  consoleln("             | %bin | %code | %prog | %progx | %in | %out");
  consoleln("  ----------------------------------------------------------------");
  consoleln("  -o         |  ✔   |       |       |   ✔    |     |     ");
  consoleln("  --compile  |  ✔   |   ✔   |   ✔   |        |     |     ");
  consoleln("  --run      |  ✔   |   ✔   |   ✔   |   ✔    |  ✔  |   ✔ ");

  consoleln();
  consoleln("  For example:");
  consoleln("    Java compile : javac -d %bin %code");
  consoleln("    Java run     : java -classpath %bin %progx < %in > %out");
  consoleln("    Python run   : python %code < %in > %out");

  consoleln();
  consoleln("More documentation: https://github.com/neungkl/instant-grader");

  consoleln();
}
