#include "../src/console.h"
#include "../src/util.h"
#include <cstring>

void test(char* expect, char* to) {
  if(strcmp(expect,to) == 0) {
    consoleln(" Pass ", Check | Green | Bold);
    console("  Except : "); consoleln(expect);
  } else {
    consoleln(" Reject ", Cross | Red | Bold);
    console("  Except : "); consoleln(expect);
    console("  Result : "); consoleln(to);
  }
}

void header(char* head) {
  consoleln();
  consoleSpliter('=');
  consoleln(head, Bold);
  consoleln();
}

int main() {

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

  consoleln();
}
