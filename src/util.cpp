#include "util.h"
#include <stdexcept>
#include <cstring>

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
  Replace the substring with given pattern
  @param str String to replace
  @param pattern Pattern condition
  @param to Convert to what string
  @return String result
*/
char* strReplace(char* str, const char* pattern, const char* to) {
  int countReplace = 0;

  int i,j,k;

  int lenStr = strlen(str);
  int lenPtt = strlen(pattern);
  int lenTo  = strlen(to);

  for(i = 0; i < lenStr; ++i) {
    for(j = 0; j < lenPtt && i + j < lenStr; ++j) {
      if(str[i + j] != pattern[j]) {
        break;
      }
    }

    // match
    if(j == lenPtt) {
      i += lenPtt - 1;
      ++countReplace;
    }
  }

  char* newStr = new char[lenStr + (lenTo - lenPtt) * countReplace + 1];

  for(i = 0, k = 0; i < lenStr; ++i, ++k) {
    for(j = 0; j < lenPtt && i + j < lenStr; ++j) {
      if(str[i + j] != pattern[j]) break;
    }

    // match
    if(j == lenPtt) {
      for(j = 0; j < lenTo; ++j, ++k) {
        newStr[k] = to[j];
      }
      i += lenPtt - 1;
      --k;
    }
    else {
      newStr[k] = str[i];
    }
  }
  newStr[k] = '\0';

  return newStr;
}

/*
  Convert integer to string
  @param num Number to convert
  @return String result
*/
char* toString(int num) {

  if(num == 0) return "0";

  int len = 0;
  int tmp = num;

  while(tmp > 0) {
    tmp /= 10;
    len++;
  }

  char* str = new char[len + 1];
  for(int i = len - 1; i >= 0; --i) {
    str[i] = (num % 10) + '0';
    num /= 10;
  }
  str[len] = '\0';

  return str;
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
      throw std::runtime_error(strReplace("%s can't open.", "%s", fileName));
    }

    return handler;
}
