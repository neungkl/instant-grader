import java.util.Scanner;

public class sumN {
  public static void main(String[] args) {

    Scanner in = new Scanner(System.in);
    int N = in.nextInt();
    int sum = 0;

    for(int i=0; i<N; i++) {
      sum += in.nextInt();
    }

    System.out.println(sum);
  }
}

/*
===
5
1 2 3 4 5
---
15
===
3
111 222 333
---
666
===
*/
