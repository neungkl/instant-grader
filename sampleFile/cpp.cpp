#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

using namespace std;

int main() {
  vector<string> hello;

  int N;
  cin >> N;

  for(int i=0; i<N; i++) hello.push_back("Hello");

  for(int i=0; i<(int) hello.size(); i++) cout << hello[i] << endl;
}

/*
===
1
---
Hello
===
5
---
Hello
Hello
Hello
Hello
Hello
===
*/
