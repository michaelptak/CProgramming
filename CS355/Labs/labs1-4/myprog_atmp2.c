#include <stdio.h>

int main(int ac, char *av[]) {

    for (int i = 1; i <ac; i++) {
      char *str = av[i];
      int index = 0;
      char filtered[100]; // temp array stores only letters

      for (int j=0; str[j] != '\0'; j++){
        if ((str[j] >= 'a' && str[j] <= 'z') || (str[j] >= 'A' && str[j] <= 'Z')) {
          filtered[index++] = str[j];
        }
      }

      // print fronm filtered string
      for (int k=0; k < index; k++) {
        for (int m =0; m <=k;m++) {
          printf("%c", filtered[m]);
        }
        printf("\n");
      }
    }

    return 0;
}
