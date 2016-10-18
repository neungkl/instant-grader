#include "console.h"
#include "grade.h"
#include "config.h"
#include <exception>

using namespace std;

void initConfig() {
  setConf(TEST_INPUT_DELIMITER,   "===");
  setConf(TEST_OUTPUT_DELIMITER,  "---");

  setConf(LANGUAGE, "c++");

  // Set compiler command
  setConf(COMPILER_CPP,   "g++ -o %out %in");
  setConf(COMPILER_C,     "gcc -o %out %in");
}

int main(int argc, char** argv) {

  int argn = 1;

  while(true && argn < argc) {
    break;

    ++argn;
  }

  if(argc - argn == 0) {
    console("", Cross | Red);
    consoleln(" Please enter specific file to run test.");
    console("  For Example: ");
    consoleln("./grader add.cpp", Bold);
  }
  else if(argc - argn == 1) {
    initConfig();

    try {
      GradeStatus status = gradeFile(argv[argc - 1]);

      if(status == FileNotFound) {
        console(" ", Cross | Red);
        console(argv[argc - 1], Bold);
        consoleln(" does not exits.");
      } else if(status == InputIsNotFile) {
        console(" ", Cross | Red);
        consoleln("Input file should not be directory.");
      } else if(status == CompileFailed) {
        console(" ", Cross | Red);
        consoleln("compile error");
      }
    } catch (exception e) {
      console(" ", Cross | Red);
      consoleln("Runtime Error.");
    }
  }
  else {
    console(" ", Cross | Red);
    consoleln("argument is invalid.");
  }

  return 0;
}
