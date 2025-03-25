#include <stdio.h>

int fizzbuzz(int);

int main() {
  int input = 0;
  printf("Please enter an integer: ");
  scanf("%d", &input);

  if (input > 0) {
    for(int i=0; i<=input;i++) {
      fizzbuzz(i);
    }
  } 
  else {
    for(int i=0; input<=i;i--) {
      fizzbuzz(i);
    }
  }
}

int fizzbuzz(int x) {
  if(x % 3 == 0) {
    printf("Fizz");
    if (x % 5 == 0) {
      printf("Buzz");
    }
  }
  else if (x % 5 == 0) {
    printf("Buzz");
  }
  else {
    printf("%d",x);
  }
  printf("\n");
}
