#include "../src/console.h"
#include "../src/util.h"
#include <cstring>
#include <stdexcept>

int sampleCount = 0;
int passCount = 0;

int isAllPass = true;

void showResult(bool pass, const char* to, const char* expect) {

  isAllPass = isAllPass && pass;

  if(pass) {
    consoleln(" Pass ", Check | Green | Bold);
    console("  Expect : "); consoleln(expect);
    ++passCount;
  } else {
    consoleln(" Reject ", Cross | Red | Bold);
    console("  Expect : "); consoleln(expect);
    console("  Result : "); consoleln(to);
  }

  ++sampleCount;
}

void testc(char* to, char* expect) {
  showResult(strcmp(to,expect) == 0, to, expect);
}

void test(bool to, bool expect) {
  showResult(to == expect, to ? "true" : "false", expect ? "true" : "false");
}

void header(char* head) {
  consoleSpliter('=', 50, Grey);
  consoleln(head, Bold);
}

int main() {

  header("Util -> isHeadEqual");

  test(isHeadEqual("aaa", "aaa"), true);
  test(isHeadEqual("aaa", "aab"), false);
  test(isHeadEqual("xxx", "xx"), true);
  test(isHeadEqual("xx", "xxx"), false);

  test(isHeadEqual("123456", "123"), true);
  test(isHeadEqual("234567", "235"), false);

  header("Util -> ToString");

  testc(toString(0), "0");
  testc(toString(1), "1");
  testc(toString(555), "555");
  testc(toString(123456789), "123456789");

  header("Util -> StrReplace");

  testc(strReplace("aaabbbccc", "aaa", "ddd"), "dddbbbccc");
  testc(strReplace("abcabcabc", "a", "d"), "dbcdbcdbc");
  testc(strReplace("abcabcabc", "abc", "x"), "xxx");

  testc(strReplace("aaaaaaaaaaaaa", "aaaaa", "x"), "xxaaa");

  testc(strReplace("aaa", "a", "xxxxxxxxxx"), "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  testc(strReplace("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", "xxxxxxxxxx", "a"), "aaa");

  header("Util -> CloneConstChar");

  testc(cloneConstChar("test"), "test");
  testc(cloneConstChar("123456789123456789123456789123456789"), "123456789123456789123456789123456789");

  consoleSpliter('=', 50, Grey);
  if(passCount < sampleCount) {
    console(" Some reject ", Cross | Red | Bold);
  } else {
    console(" All pass ", Check | Green | Bold);
  }
  printf("(%d/%d)\n", passCount, sampleCount);

  consoleln();

  test(true, false);
  test(false, true);

  if(!isAllPass) {
    return 1;
  }
  return 0;
}
