#include "../src/console.h"
#include "../src/util.h"
#include <cstring>

int sampleCount = 0;
int passCount = 0;

void showResult(bool pass, const char* to, const char* expect) {
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

void test(char* to, char* expect) {
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

  test(toString(0), "0");
  test(toString(1), "1");
  test(toString(555), "555");
  test(toString(123456789), "123456789");

  header("Util -> StrReplace");

  test(strReplace("aaabbbccc", "aaa", "ddd"), "dddbbbccc");
  test(strReplace("abcabcabc", "a", "d"), "dbcdbcdbc");
  test(strReplace("abcabcabc", "abc", "x"), "xxx");

  test(strReplace("aaaaaaaaaaaaa", "aaaaa", "x"), "xxaaa");

  test(strReplace("aaa", "a", "xxxxxxxxxx"), "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
  test(strReplace("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx", "xxxxxxxxxx", "a"), "aaa");

  header("Util -> CloneConstChar");

  test(cloneConstChar("test"), "test");
  test(cloneConstChar("123456789123456789123456789123456789"), "123456789123456789123456789123456789");

  consoleSpliter('=', 50, Grey);
  if(passCount < sampleCount) {
    console(" Some reject ", Cross | Red | Bold);
  } else {
    console(" All pass ", Check | Green | Bold);
  }
  printf("(%d/%d)\n", passCount, sampleCount);

  consoleln();
}
