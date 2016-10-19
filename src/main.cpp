#include "console.h"
#include "grade.h"
#include "config.h"
#include <stdexcept>
#include <cstring>

using namespace std;

void initConfig() {
  setConf(TEST_INPUT_DELIMITER,   "===");
  setConf(TEST_OUTPUT_DELIMITER,  "---");

  setConf(CLI_VERBOSE, false);

  // Set compiler command
  setCompileCmd("c",    "gcc -o %out %in");
  setCompileCmd("c++",  "g++ -o %out %in");
  setCompileCmd("java", "javac -d %bin %in");

  setRunCmd("java", "java -classpath %progd %progx < %in > %out");
}

bool readParamter(int &argn, int argc, char** argv) {
  while(argn < argc) {

    if(strcmp(argv[argn], "-v") == 0) {
      setConf(CLI_VERBOSE, true);
    }
    else {
      break;
    }

    ++argn;
  }

  return true;
}

int main(int argc, char** argv) {

  initConfig();

  int argn = 1;
  int fileIndex = -1;

  if(argc == 1) {
    console("", Cross | Red);
    consoleln(" Please enter specific file to run test.");
    console("  For Example: ");
    consoleln("./grader add.cpp", Bold);
  } else {
    int isParamValid = true;

    isParamValid = isParamValid && readParamter(argn, argc, argv);

    fileIndex = argn++;

    isParamValid = isParamValid && readParamter(argn, argc, argv);

    if(!isParamValid) {
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
