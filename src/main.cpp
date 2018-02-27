#include "console.h"
#include "grade.h"
#include "config.h"
#include <stdexcept>
#include <cstring>

#include <cstdio>
#ifdef WINDOWS
  #include <windows.h>
#else
  #include <cstdlib>
#endif
#define PATH_LEN 512

using namespace std;

char binPath[PATH_LEN];
char currentPath[PATH_LEN];

void initConfig() {

  setConf(TEST_INPUT_DELIMITER,   "===");
  setConf(TEST_OUTPUT_DELIMITER,  "---");

  setConf(CLI_VERBOSE, false);
  setConf(HELP, false);

  sprintf(binPath, "%s/bin", currentPath);
  binPath[sizeof(binPath) - 1] = '\0';

  setConf(BIN_PATH, binPath);
  setConf(PROGRAM_PATH, "%bin/%progx");

  // Set compiler command
  setCompileCmd("default",  "gcc -Wall -o \"%prog\" \"%code\"");
  setCompileCmd("c",        "gcc -Wall -o \"%prog\" \"%code\"");
  setCompileCmd("c++",      "g++ -Wall -o \"%prog\" \"%code\"");
  setCompileCmd("java",     "javac -d \"%bin\" \"%code\"");
  setCompileCmd("python",   "");
  setCompileCmd("haskell",  "ghc -o \"%prog\" \"%code\"");
  setCompileCmd("ruby",     "");

  setRunCmd("default",  "\"%prog\" < \"%in\" > \"%out\"");
  setRunCmd("java",     "java -classpath \"%bin\" \"%progx\" < \"%in\" > \"%out\"");
  setRunCmd("python",   "python \"%code\" < \"%in\" > \"%out\"");
  setRunCmd("ruby",     "ruby \"%code\" < \"%in\" > \"%out\"");
}

bool readParamter(int &argn, int argc, char** argv) {
  while(argn < argc) {

    if(strcmp(argv[argn], "-v") == 0) {
      setConf(CLI_VERBOSE, true);
    }
    else if(strcmp(argv[argn], "-in") == 0) {
      setConf(TEST_INPUT_DELIMITER, argv[++argn]);
    }
    else if(strcmp(argv[argn], "-out") == 0) {
      setConf(TEST_OUTPUT_DELIMITER, argv[++argn]);
    }
    else if(strcmp(argv[argn], "--compile") == 0) {
      setConf(LANGUAGE, "custom");
      setCompileCmd("custom", argv[++argn]);
    }
    else if(strcmp(argv[argn], "--run") == 0) {
      setConf(LANGUAGE, "custom");
      setRunCmd("custom", argv[++argn]);
    }
    else if(strcmp(argv[argn], "-o") == 0) {
      int len = strlen(argv[++argn]);

      // Remove tail /
      if(argv[argn][len - 1] == '/') argv[argn][len - 1] = '\0';
      setConf(PROGRAM_PATH, argv[argn]);
    }
    else if(strcmp(argv[argn], "-b") == 0) {
      int len = strlen(argv[++argn]);
      if(argv[argn][len - 1] == '/') argv[argn][len - 1] = '\0';
      setConf(BIN_PATH, argv[argn]);
    }
    else if(strcmp(argv[argn], "-h") == 0) {
      setConf(HELP, true);
    }
    else {
      break;
    }

    ++argn;
  }

  return true;
}

int main(int argc, char** argv) {

  char slashChr = '\\';

  #ifdef WINDOWS
    GetFullPathName(__FILE__, currentPath, PATH_LEN);
  #else
    realpath(__FILE__, currentPath);
    slashChr = '/';
  #endif

  if (strlen(currentPath) == 0)
  {
    console(" ", Cross | Red);
    consoleln("Can't get current directory of grader");
    return -1;
  }

  char *lastSlash = strrchr(currentPath, slashChr);
  *lastSlash = '\0';
  lastSlash = strrchr(currentPath, slashChr);
  *lastSlash = '\0';

  initConfig();

  int argn = 1;
  int fileIndex = -1;

  if(argc == 1) {
    console("", Cross | Red);
    consoleln(" Please enter specific file to run test.");
    console("  For Example: ");
    consoleln("instant-grader add.cpp", Bold);
  } else {
    int isParamValid = true;

    isParamValid = isParamValid && readParamter(argn, argc, argv);

    fileIndex = argn++;

    isParamValid = isParamValid && readParamter(argn, argc, argv);

    if(confi(HELP)) {
      consoleHelp();
    } else if(!isParamValid) {
      console(" ", Cross | Red);
      consoleln("argument is invalid.");
    } else if((fileIndex < argc - 1 && argn == argc) || fileIndex == argc - 1) {
      try {
        GraderStatus status = gradeFile(argv[fileIndex]);

        if(status == FileNotFound) {
          console(" ", Cross | Red);
          console(argv[fileIndex], Bold);
          consoleln(" does not exits.");
        } else if(status == InputIsNotFile) {
          console(" ", Cross | Red);
          consoleln("Input file should not be directory.");
        } else if(status == CompileFailed) {
          console(" ", Cross | Red);
          consoleln("compile error");
        }
      } catch (runtime_error e) {
        consoleln(e.what());
        console(" ", Cross | Red);
        consoleln("Runtime Error.");
      }

    } else {
      console(" ", Cross | Red);
      consoleln("argument is invalid.");
    }
  }

  return 0;
}
