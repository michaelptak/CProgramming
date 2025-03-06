#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void do_cat(char*);
#define BUFFER_SIZE 1024
int flagE=0, flagN = 0, flagS = 0;

int main(int ac, char* av[]) {
  char option;
  
   while ((option = getopt(ac, av, "ens")) != -1) {
    switch (option) {
      case 'e':
        flagE = 1;
        printf("Flag E enabled\n");
        break;
      case 'n':
        flagN = 1;
        printf("Flag N enabled\n");
        break;
      case 's':
        flagS = 1;
        printf("Flag S enabled\n");
        break;
      default:
        printf("Usage: mycat [-ens] file1 [file2 ...]\n");
        return 1;
    }
  }

  if (ac == 1) {
    printf("Usage: mycat [-ens] file1 [file2 ...]\n");
  } else {
    while (--ac) {
      printf("%s\n", *++av);
      do_cat(*av);
    }
  }

  return 0; 
}


void do_cat(char *filename) {
  int fd = open(filename, O_RDONLY);
  if (fd == -1) {
    perror("Error opening file");
    return;
  }

  char buffer[BUFFER_SIZE];
  int numBytes;

  while((numBytes=read(fd, buffer, sizeof(buffer))) > 0) {
    for(int i =0; i<numBytes; i++) {
      if (flagE && buffer[i] == '\n') {
        printf("$\n");
      } else {
        printf("%c", buffer[i]);
      }


    }

  }

  close(fd);
}
