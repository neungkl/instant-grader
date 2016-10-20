#include "grade.h"
#include "config.h"
#include "console.h"
#include "compare.h"
#include "global.h"
#include "util.h"
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>
#include <cstdlib>

#include <cstdio>

char* binName = "./bin";
char* binFileInput  = "%bin/%d.in";
char* binFileOutput = "%bin/%d.out";
char* binFileAnswer = "%bin/%d.ans";

/*
  Create file handler with specific given test case number
  @param fileName current file name
  @param access access context of the file
  @param testCaseNumber number of test case
  @return FILE pointer
*/
FILE* createFileHandlerWithTestCase(char* fileName, char* access, int testCaseNumber) {
  char* testCaseFileName = strReplace(fileName, "%bin", binName);
  testCaseFileName = strReplace(testCaseFileName, "%d", toString(testCaseNumber));
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
void generateCompileMsg(char* &cstr, char* code, char* codex, char* prog) {
  cstr = strReplace(cstr, "%codex", codex);
  cstr = strReplace(cstr, "%code", code);
  cstr = strReplace(cstr, "%prog", prog);
  cstr = strReplace(cstr, "%bin", binName);
}

/*
  Remove extension form file name (for generating execute program name)
  @param fileName file name to remove extension
  @return array of 1) file name and 2) extension
*/
char** splitFileExtension(char* fileName) {

  char** result = new char*[2];

  int i,j;
  int len, first, last;
  len = strlen(fileName);

  result[0] = new char[len];
  result[1] = new char[len];

  first = -1;
  for(i = 0; i < len; ++i) {
    if(fileName[i] == '/') first = i + 1;
  }

  if(first == -1) first = 0;

  for(last = len - 1; fileName[last] != '.' && last >= first; --last);

  if(last < first) {
    result[1][0] = '\0';
    last = len;
  }

  for(i = last + 1, j = 0; i < len; ++i, ++j) result[1][j] = fileName[i];
  result[1][len - 1 - last] = '\0';

  last = last - 1;
  for(i = first, j = 0; i <= last; ++i, ++j) result[0][j] = fileName[i];
  result[0][j] = '\0';

  return result;
}

/*
  Compile the code
  @param in Input file to compile
  @param out Output file of executable program
  @return status code of compile
*/
int compileCode(char* in, char* inx, char* out) {

  char* compileStr = getCompileCmd(conf(LANGUAGE));

  generateCompileMsg(compileStr, in, inx, out);

  console("Compile: ", Bold);
  consoleln(compileStr, Bold);

  return system(compileStr);
}

/*
  Run all test subject
  @param programFile the executable run file
  @param numberOfTest number of test
*/
void runTest(char* programPath, char* programName, int numberOfTest) {
  char* fileInputName;
  char* fileAnswerName;
  char* runCommand;

  char* programFile = strReplace("pathname", "path", programPath);
  programFile = strReplace(programFile, "name", programName);

  const int subSplitterWidth = 20;

  FILE *fin, *fout, *fans;

  bool isVerbose = confi(CLI_VERBOSE);

  if(strcmp(getRunCmd(conf(LANGUAGE)), getRunCmd("c")) != 0) {

    runCommand = getRunCmd(conf(LANGUAGE));
    runCommand = strReplace(runCommand, "%progd", programPath);
    runCommand = strReplace(runCommand, "%progx", programName);
    runCommand = strReplace(runCommand, "%prog", programFile);

    consoleln(strReplace("Run with command: %s", "%s", runCommand), Bold);
  }

  for(int N = 1; N <= numberOfTest; ++N) {
    fileInputName = strReplace(binFileInput, "%bin", binName);
    fileInputName = strReplace(fileInputName, "%d", toString(N));

    fileAnswerName = strReplace(binFileAnswer, "%bin", binName);
    fileAnswerName = strReplace(fileAnswerName, "%d", toString(N));

    console(strReplace("Run case: %d ", "%d", toString(N)));

    runCommand = getRunCmd(conf(LANGUAGE));
    runCommand = strReplace(runCommand, "%progd", programPath);
    runCommand = strReplace(runCommand, "%progx", programName);
    runCommand = strReplace(runCommand, "%prog", programFile);
    runCommand = strReplace(runCommand, "%in", fileInputName);
    runCommand = strReplace(runCommand, "%out", fileAnswerName);

    int status = system(runCommand);

    if(status != 0) {
      console(" ", Cross | Red | Bold);
      consoleln("Rejected", Red | Bold);

      consoleln();
      console("Error: ", Red | Bold);
      consoleln(strReplace("You program exit with status code %d", "%d", toString(status)));
      if(status > 35000) consoleln("Maybe stack overflow or using too much memory.");
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

      if(isVerbose) {
        consoleSpliter('-', subSplitterWidth, Grey);
        consoleFile(fin);
        consoleSpliter('-', subSplitterWidth, Grey);
        consoleFile(fout);
      }
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

    if( N < numberOfTest && (!pass || isVerbose) ) consoleSpliter('-', 40);
  }

}

/*
  Grade the current file
  @param fileName file to grade
  @return status of grading
*/
GraderStatus gradeFile(char* fileNamePath) {

  GraderStatus status = Pass;

  struct stat fileInfo;

  if( stat(fileNamePath, &fileInfo) != 0 ) {
    return FileNotFound;
  } else if(fileInfo.st_mode & S_IFDIR) {
    return InputIsNotFile;
  }

  FILE *pFile = fopen(fileNamePath, "r");

  if(pFile != NULL) {

    int numberOfTest = 0;

    /* Extract content to individual file */
    numberOfTest = splitTestCase(pFile);
    fclose(pFile);

    char** fileNameSplit = splitFileExtension(fileNamePath);
    char* fileNameWOExtension = fileNameSplit[0];
    char* extension = fileNameSplit[1];

    setLanguage(extension);

    /* Compile the code */
    char* programFileName = strReplace("%bin/%s", "%bin", binName);

    consoleln();

    // Compile when there has compile message
    if(strcmp(getCompileCmd(conf(LANGUAGE)),"") != 0) {

      programFileName = strReplace(programFileName, "%s", fileNameWOExtension);

      int compileStatus = compileCode(fileNamePath, fileNameWOExtension, programFileName);

      if(compileStatus != 0) {
        return CompileFailed;
      } else {
        console(" ", Check | Green);
        console("Complete: ", Green | Bold);
        consoleln("No error message.");
      }

      consoleSpliter('=', 40);

      /* Run code with all test files */
      runTest(strReplace("%s/", "%s", binName), fileNameWOExtension, numberOfTest);
    } else {
      /* Run code with all test files */
      runTest("", fileNamePath, numberOfTest);
    }


    consoleln();

  } else {
    return FileNotFound;
  }

  return status;
}
