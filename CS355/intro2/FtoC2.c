#include <stdio.h>

int main() {
   
  float fCelsius=0, fFarenheit=0;
  printf("Please enter the temperate in Farenheit: ");
  scanf("%f", &fFarenheit);
  fCelsius = (fFarenheit-32)/9*5;
  printf("%5.1fF is %2.1fC\n", fFarenheit, fCelsius);



  return 0;
}
