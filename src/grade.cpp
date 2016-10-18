#include "grade.h"
#include "config.h"
#include "console.h"
#include "compare.h"
#include "global.h"
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <exception>
#include <cstdlib>
#include <cstdio>

using namespace std;

char* binFileInput  = "./bin/%d.in";
char* binFileOutput = "./bin/%d.out";
char* binFileAnswer = "./bin/%d.ans";

/*
  Compare the head of given content with head parameter
  @param content content to compare
  @param head head string
  @return boolean
*/
bool isHeadEqual(char* content, char* head) {
  for(int i=0; head[i] != '\0'; ++i) {
    if(head[i] != content[i]) return false;
  }
  return true;
}

/*
  Create file handler
  @param fileName current file name
  @param access access context of the file
  @return FILE pointer
*/
FILE* createFileHandler(char* fileName, char* access) {
    FILE* handler = fopen(fileName, access);

    if(handler == NULL) {
      perror("fopen");
      throw exception();
    }

    return handler;
}

/*
  Create file handler with specific given test case number
  @param fileName current file name
  @param access access context of the file
  @param testCaseNumber number of test case
  @return FILE pointer
*/
FILE* createFileHandlerWithTestCase(char* fileName, char* access, int testCaseNumber) {
  char testCaseFileName[strlen(fileName) + 5];
  sprintf(testCaseFileName, fileName, testCaseNumber);
  return createFileHandler(testCaseFileName, access);
}

/*
  Read content from pFile and split to individual test case file
  @param pFile file to split test case
  @return number of test case
*/
int splitTestCase(FILE* pFile) {

  int testCase = 0;

  enum FileContentState {
    Code,
    TestInput,
    TestOutput
  };

  mkdir("./bin/", 0766);

  FileContentState fcState = Code;
  char content[BUFFER_SIZE + 1];

  char* inputDelimiter = conf(TEST_INPUT_DELIMITER);
  char* outputDelimiter = conf(TEST_OUTPUT_DELIMITER);

  FILE *finput, *foutput;

  while(fgets(content, BUFFER_SIZE, pFile) != NULL) {

    if(fcState == Code) {
      if(isHeadEqual(content, inputDelimiter)) {
        fcState = TestInput;

        ++testCase;
        finput = createFileHandlerWithTestCase(binFileInput, "w+", testCase);
        foutput = createFileHandlerWithTestCase(binFileOutput, "w+", testCase);
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
        fputs(content, finput);
      }
    }
    else if(fcState == TestOutput) {
      if(isHeadEqual(content, inputDelimiter)) {

        fclose(foutput);
        fcState = TestInput;

        ++testCase;
        finput = createFileHandlerWithTestCase(binFileInput, "w+", testCase);
        foutput = createFileHandlerWithTestCase(binFileOutput, "w+", testCase);
      }
      else if(isHeadEqual(content, outputDelimiter)) {
        continue;
      }
      else {
        fputs(content, foutput);
      }
    }

  }

  return testCase > 0 ? testCase - 1 : 0;
}

/*
  Replace %in and %out compile string with specify text
  @param cstr raw compile string
  @param in input string to replace %in
  @param out output string to replace %out
*/
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

/*
  Remove extension form file name (for generating execute program name)
  @param fileName file name to remove extension
  @return file name that already exclude extension
*/
char* excludeExtension(char* fileName) {

  int len, i;
  len = i = strlen(fileName);
  char* newName = new char[len];

  for(--i; fileName[i] != '.'; --i);
  --i;

  if(i < 0) i = len - 1;

  newName[i + 1] = '\0';
  for(; i >= 0; --i) {
    newName[i] = fileName[i];
  }

  return newName;
}

/*
  Compile the code
  @param in Input file to compile
  @param out Output file of executable program
  @return status code of compile
*/
int compileCode(char* in, char* out) {
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

/*
  Run all test subject
  @param programFile the executable run file
  @param numberOfTest number of test
*/
void runTest(char* programFile, int numberOfTest) {
  char fileInputName[MAX_NAME_LEN];
  char fileAnswerName[MAX_NAME_LEN];
  char runCommand[MAX_NAME_LEN];

  const int subSplitterWidth = 20;

  FILE *fin, *fout, *fans;

  char logs[MAX_NAME_LEN];

  for(int N = 1; N <= numberOfTest; ++N) {
    sprintf(fileInputName , binFileInput , N);
    sprintf(fileAnswerName, binFileAnswer, N);

    sprintf(logs, "Run case: %d ", N);
    console(logs);

    sprintf(runCommand, "%s < %s > %s", programFile, fileInputName, fileAnswerName);
    int status = system(runCommand);

    if(status != 0) {
      console("Error: ", Red | Bold);
      sprintf(logs, "You program exit with status code %d", status);
      consoleln(logs);
      break;
    }

    fin = NULL;
    fout = createFileHandlerWithTestCase(binFileOutput, "r", N);
    fans = createFileHandlerWithTestCase(binFileAnswer, "r", N);

    int pass = compare(fout, fans);

    if(pass) {
      console(" ", Check | Green | Bold);
      consoleln("Accepted", Green | Bold);

      fin = createFileHandlerWithTestCase(binFileInput, "r", N);

      consoleSpliter('-', subSplitterWidth, Grey);
      consoleFile(fin);
      consoleSpliter('-', subSplitterWidth, Grey);
      consoleFile(fout);
    } else {
      console(" ", Cross | Red | Bold);
      consoleln("Rejected", Red | Bold);

      fin = createFileHandlerWithTestCase(binFileInput, "r", N);

      consoleSpliter('-', subSplitterWidth, Grey);
      consoleFile(fin);
      consoleSpliter('-', subSplitterWidth, Grey);
      consoleFile(fout);
      consoleSpliter('-', subSplitterWidth, Grey);
      consoleln("Your answer:");
      consoleFile(fans);
    }

    if(fin != NULL) fclose(fin);
    fclose(fout);
    fclose(fans);

    if( N < numberOfTest ) consoleSpliter('=', 40);
  }

}

/*
  Grade the current file
  @param fileName file to grade
  @return status of grading
*/
GradeStatus gradeFile(char* fileName) {

  GradeStatus status = Pass;

  struct stat fileInfo;

  if( stat(fileName, &fileInfo) != 0 ) {
    return FileNotFound;
  } else if(fileInfo.st_mode & S_IFDIR) {
    return InputIsNotFile;
  }

  FILE *pFile = fopen(fileName, "r");

  if(pFile != NULL) {

    int numberOfTest = 0;

    /* Extract content to individual file */
    numberOfTest = splitTestCase(pFile);
    fclose(pFile);

    /* Compile the code */
    char* programFileName;
    sprintf(programFileName, "./bin/%s", excludeExtension(fileName));

    int compileStatus = compileCode(fileName, programFileName);

    if(compileStatus != 0) {
      return CompileFailed;
    } else {
      console(" ", Check | Green);
      console("Complete: ", Green | Bold);
      consoleln("No error message.");
    }

    consoleSpliter('=', 40);

    /* Run code with all test files */
    runTest(programFileName, numberOfTest);
    consoleln();

  } else {
    return FileNotFound;
  }

  return status;
}
