#include "console.h"
#include "grade.h"
#include "config.h"

void initConfig() {
  setConf("test-delimiter",   "===");
  setConf("inout-delimiter",  "---");

  setConf("compile-lang", "c++");

  // Set compiler command

  setConf("compiler-c++",   "g++ -o %out %in");
  setConf("compiler-c",     "gcc -o %out %in");
}

int main(int argc, char** argv) {

  int argn = 1;

  while(true && argn < argc) {
    break;

    ++argn;
  }

  if(argc - argn == 0) {
    console("Please enter specific file to run test.\n", Red | Bold);
    console("For Example : ");
    console("./grader add.cpp\n", Bold);
  }
  else if(argc - argn == 1) {
    initConfig();

    GradeStatus status = gradeFile(argv[argc - 1]);

    //console(conf("compiler-c++"));
  }
  else {
    console("This grader currently support only single file.\n", Red | Bold);
  }

  return 0;
}
