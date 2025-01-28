#include <stdio.h>

int main() {
  int iAge = 0;
  printf("Please enter your age: ");
  int iReturnValue = scanf("%d", &iAge);
  if(iReturnValue!= 1) {
    printf("scanf() could not interpret you input\n");
    return 1;
  }
  //printf("You told me that you are %d years old\n", iAge);
  
  if(iAge >= 0 && iAge <= 120) {
    if(iAge < 21)
      printf("You cannot legally enjoy alcoholic beverages\n");
    else 
      printf("Hello! Your age is %d\n", iAge);
  } 
  else {
    printf("%d is not a valid age\n", iAge);
  }

  return 0;
}

