#include <stdio.h>

int main(int ac, char *av[]) {

    for (int i = 1; i <ac; i++) {
      char *str = av[i];
      int index = 0;

      while (str[index] != '\0') {
        for (int j=0; j <=index; j++) {
          if ((str[j] >= 'a' && str[j] <= 'z') || (str[j] >= 'A' && str[j] <= 'Z')) {
            printf("%c", str[j]);
          }
        }
        index++;
        printf("\n");
      }
    }

    return 0;
}
