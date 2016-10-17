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
    console("", Cross | Red | Bold);
    consoleln(" Please enter specific file to run test.", Red | Bold);
    console("For Example: ");
    consoleln("./grader add.cpp", Bold);
  }
  else if(argc - argn == 1) {
    initConfig();

    try {
      GradeStatus status = gradeFile(argv[argc - 1]);

      if(status == FileNotFound) {
        console("  ", Cross);
        console(argv[argc - 1], Bold);
        consoleln(" does not exits.");
      } else {

      }
    } catch (exception e) {
      consoleln("Runtime Error.", Red | Bold);
    }
  }
  else {
    console("This grader currently support only single file.\n", Red | Bold);
  }

  return 0;
}
