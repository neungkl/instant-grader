#include "compare.h"
#include "global.h"
#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;

/*
  Compare content of two file.
  This algorithm will exclude all space, tab and newline.
  @param a first file
  @param b second file
  @return status if file are same
*/
bool compare(FILE* a, FILE* b) {

  char buffA[BUFFER_SIZE + 1];
  char buffB[BUFFER_SIZE + 1];

  buffA[BUFFER_SIZE] = buffB[BUFFER_SIZE] = '\0';

  rewind(a);
  rewind(b);

  while(1) {
    bool eofA = fscanf(a, BUFFER_IO_STRING, buffA) == EOF;
    bool eofB = fscanf(b, BUFFER_IO_STRING, buffB) == EOF;

    if(eofA != eofB) return false;
    if(eofA && eofB) break;

    if(strcmp(buffA, buffB) != 0) return false;
  }

  return true;
}
