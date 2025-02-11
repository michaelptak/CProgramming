#include <stdio.h>
#include <string.h>

int main(){
  char *string1 = "Hello, world!";
  printf("string1: %s\n", string1);
  printf("string1[5]: %c\n", string1[5]);
  //string1[5] = " ";
  //printf("string1[5]: %c\n", string1[5]);

  char string2[25] = "Hello, world!";
  printf("string2: %s\n",string2);
  for(int i=0; string2[i] != 0; i++) {
    printf("string[%d]: %c (%d)\n", i, string2[i], string2[i]);
  }

  //Copying a string
  string1 = string2;
  string1[5] = ' ';
  printf("string1: %s\n", string1);
  printf("string2: %s\n", string2);

  return 0;
}
