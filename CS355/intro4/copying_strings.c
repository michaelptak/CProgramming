#include <stdio.h>
#include <string.h>

// several versions of implementation

// array subscript version with explicit copy
void strcpy1(char *dest, const char *src) {
  int i=0;
  while(src[i]!=0) {
    dest[i] = src[i];
    i++;
  }
  dest[i];
}

int main() {
  char str1[100] = "Hello, world!";
  char str2[100];
}
