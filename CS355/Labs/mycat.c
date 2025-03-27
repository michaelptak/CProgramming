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

  if (optind >= ac) {
    printf("Usage: mycat [-ens] file1 [file2 ...]\n");
  }
  for (int i = optind; i < ac; i++) {
    printf("Reading from file: %s\n", av[i]);
    do_cat(av[i]);
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
  int lineNum = 1;
  int atLineStart = 1; // flag to track if at start of line
  int emptyLine = 1; //track if empty line
  int prevLineEmpty = 0;// track if prev line empty'

  while((numBytes = read(fd, buffer, sizeof(buffer))) > 0) {
    for(int i = 0; i < numBytes; i++) {
      // Handle flag N and check for whitespace
      if (atLineStart) {
        // Check for emptyy line 
        if (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\r' && buffer[i] != '\n') {
          emptyLine = 0;
        }

        if (flagN && (!flagS || !emptyLine || !prevLineEmpty)) {
          printf("%2d ", lineNum++);
        }

        atLineStart = 0;
    }

      // Check for windows line ending (\r\n)
      if (buffer[i] == '\r' && i + 1 < numBytes && buffer[i + 1] == '\n') {
        // Print if: S flag not set OR Line is not empty OR line is empty but previous not empty 
        if (!flagS || !emptyLine || !prevLineEmpty) {
          if (flagE) {
            printf("$");
          }
          printf("\r\n");
        }
        
        prevLineEmpty = emptyLine;
        emptyLine = 1;  // assume next line is empty
        atLineStart = 1;           
        i++;  // skip over \n
      }

      // Check for unix line ending (\n)
      else if (buffer[i] == '\n') {
         if (!flagS || !emptyLine || !prevLineEmpty) {
          if (flagE) {
            printf("$");
          }
          printf("\n");
        }
        
        prevLineEmpty = emptyLine;
        emptyLine = 1;  
        atLineStart = 1;      
      }

      // Regular character (only print if not skipping this line)
      else if (!flagS || !emptyLine || !prevLineEmpty) {
        printf("%c", buffer[i]);    
      }
    }
  }
  close(fd);
}
