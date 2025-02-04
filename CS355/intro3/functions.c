#include <stdio.h>

int product1(int,int);
int product2(int,int);
int product3(int,int*); // Note that second value is a pointer and not the value

int main(void) {
  int a,b;
  printf("Please enter two values: ");
  scanf("%d %d", &a, &b);
  printf("You entered %d and %d\n", a, b);

  int tmp1 = product1(a, b);
  printf("Product1: %d * %d = %d.\n", a, b, tmp1);

  int tmp2 = product2(a, b);
  printf("Product2: %d * %d = %d.\n", a, b, tmp2);

  int tmp3 = product3(a, &b);
  printf("Product3: %d * %d = %d.\n", a, b, tmp3);

  return 0;
}

int product1(int x, int y){
  int result = x * y;
  return result;
}

int product2(int x, int y) {
  int result = x * y;
  x = 3;
  y = 7;
  return result;
}

int product3(int x, int *y) {
  int result = x * (*y); //pointer must be dereferenced
  x = 3;
  *y = 7;
  return result;
}
