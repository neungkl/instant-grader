#include <stdio.h>

int my_abs(int N) {
  if(N == 0) return 0;
  return 1 + my_abs(N-1);
}

int main() {
  int N;
  scanf("%d",&N);
  printf("%d", my_abs(N));
}

/*
======
5
------
5
======
-5
------
5
======
*/
