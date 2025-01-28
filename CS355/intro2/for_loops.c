#include <stdio.h>

int main(){
  for(int line = 1; line <=10; line++){
    printf("%2d ", line);
    for(int star=1; star<=line;star++){
      printf("*");
    }
    printf("\n");
  }


  return 0;
}




