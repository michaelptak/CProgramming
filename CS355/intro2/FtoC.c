#include <stdio.h>

int main() {
  int iCelsius, iFarenheit=137;
  iCelsius = (iFarenheit-32)/9*5;
  printf("%dF is %dC\n", iFarenheit, iCelsius);
  
  
  float fCelsius, fFarenheit=137;
  fCelsius = (fFarenheit-32)/9*5;
  printf("%5.1fF is %2.1fC\n", fFarenheit, fCelsius);
  return 0;
}
