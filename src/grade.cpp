#include "grade.h"
#include "config.h"
#include "console.h"
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <exception>
#include <cstdlib>

#include <cstdio>

using namespace std;

bool isHeadEqual(char* content, char* head) {
  for(int i=0; head[i] != '\0'; ++i) {
    if(head[i] != content[i]) return false;
  }
  return true;
}

void generateTestCaseFile(FILE* &input, FILE* &output, int &testCase) {

  char testCaseFileName[20];

  ++testCase;

  sprintf(testCaseFileName, "./bin/%d.in", testCase);
  input = fopen(testCaseFileName, "w+");

  if(input == NULL) {
    perror("fopen");
    throw exception();
  }

  sprintf(testCaseFileName, "./bin/%d.out", testCase);
  output = fopen(testCaseFileName, "w+");

  if(output == NULL) {
    perror("fopen");
    throw exception();
  }
}

void splitTestCase(FILE* pFile, int &testCase) {

  enum FileContentState {
    Code,
    TestInput,
    TestOutput
  };

  mkdir("./bin/", 0766);

  FileContentState fcState = Code;
  char content[1025];

  char* inputDelimiter = conf(TEST_INPUT_DELIMITER);
  char* outputDelimiter = conf(TEST_OUTPUT_DELIMITER);

  FILE *finput, *foutput;

  while(fgets(content, 1024, pFile) != NULL) {

    if(fcState == Code) {
      if(isHeadEqual(content, inputDelimiter)) {
        fcState = TestInput;

        generateTestCaseFile(finput, foutput, testCase);
      }
    }
    else if(fcState == TestInput) {
      if(isHeadEqual(content, outputDelimiter)) {
        fclose(finput);
        fcState = TestOutput;
      }
      else if(isHeadEqual(content, inputDelimiter)) {
        continue;
      }
      else {
        // Current state is writing content to input test case file
        fputs(content, finput);
      }
    }
    else if(fcState == TestOutput) {
      if(isHeadEqual(content, inputDelimiter)) {

        fclose(foutput);
        fcState = TestInput;

        generateTestCaseFile(finput, foutput, testCase);
      }
      else if(isHeadEqual(content, outputDelimiter)) {
        continue;
      }
      else {
        // Current state is writing content to output test case file
        fputs(content, foutput);
      }
    }

  }
}

void generateCompileMsg(char* &cstr, char* in, char* out) {

  int len = 0;

  int argvCount = 0;
  int maxLen = 0;

  for(int i=0; ; ++i) {
    if(cstr[i] == ' ' || cstr[i] == '\0') {
      if(++len > maxLen) maxLen = len;
      len = 0;
      ++argvCount;

      if(cstr[i] == '\0') break;
    }
    else {
      ++len;
    }
  }

  char tmp[maxLen];
  int index = 0;
  int newLen = 0;
  len = 0;

  char** argv = new char*[argvCount];

  for(int i=0; ; ++i) {
    if(cstr[i] == ' ' || cstr[i] == '\0') {
      tmp[len++] = '\0';

      if(strcmp(tmp,"%in") == 0) {
        argv[index] = new char[strlen(in)];
        sprintf(argv[index], "%s ", in);
      }
      else if(strcmp(tmp, "%out") == 0) {
        argv[index] = new char[strlen(out)];
        sprintf(argv[index], "%s ", out);
      }
      else {
        argv[index] = new char[len];
        sprintf(argv[index], "%s ", tmp);
      }
      ++index;

      newLen += len;

      len = 0;

      if(cstr[i] == '\0') break;
    } else {
      tmp[len++] = cstr[i];
    }
  }

  cstr = new char[newLen];
  strcpy(cstr, "");

  for(int i=0; ; ++i) {
    if(argv[i] == NULL) break;
    strcat(cstr, argv[i]);
  }
}

int compileCode(char* in, char* out, int testCase) {
  char* compileStr = NULL;

  char* lang = conf(LANGUAGE);

  if(strcmp(lang, "c++") == 0) {
    compileStr = conf(COMPILER_CPP);
  } else if(strcmp(lang, "c") == 0) {
    compileStr = conf(COMPILER_C);
  }

  generateCompileMsg(compileStr, in, out);

  console("Compile: ", Bold);
  consoleln(compileStr, Bold);

  return system(compileStr);
}

GradeStatus gradeFile(char* fileName) {

  GradeStatus status = Pass;

  FILE *pFile = fopen(fileName, "r");

  if(pFile != NULL) {

    int testCase = 0;

    splitTestCase(pFile, testCase);
    fclose(pFile);

    int compileStatus = compileCode(fileName, "./bin/run", testCase);
    if(compileStatus != 0) {
      return CompileFailed;
    } else {
      console(" ", Check | Green);
      consoleln("Complete: No error message.");
    }

    consoleSpliter('=');

  } else {
    status = FileNotFound;
  }

  return status;
}
