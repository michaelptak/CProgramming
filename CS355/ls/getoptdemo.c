#include <stdio.h> 
#include <unistd.h>

int flagE=0, flagN=0,flagS=0;

int main(int argc, char *argv[]){
  char option;
  while((option= getopt(argc, argv,"ens")) !=-1 ){
    switch(option){
      case 'e':
        flagE=1;
        printf("Flag E enabled\n");
        break;
      case 'n':
        flagN=1;
        printf("Flag N enabled\n");
        break;
      case 's':
        flagS=1;
        printf("Flag S enabled\n");
        break;
    }
  }
 
  return 0;
}
