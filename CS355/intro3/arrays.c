#include <stdio.h>

int main() {
  int intArray[100];

  for(int i=0; i<100;i++) {
    intArray[i] = i*2;
    printf("Array[%d] is %d\n", i, intArray[i]);
  }

  return 0;
}
