#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h> 

#define BUFSIZE 1024

//helper function to make sure prohibited words are case insenstive

void to_lowercase(char *dest, const char *src){
  for(; *src; src++, dest++) {
    *dest = tolower(*src);
  }
  *dest = '\0';
}



//check for prohibited words
int contains_prohibited_word(const char *line) {
  const char *words[] = {"apple", "pear", "banana", "orange", "plum"};
  char lower_line[BUFSIZE];

  to_lowercase(lower_line, line);

  for (int i = 0; i < 5; i++) {
    if (strstr(lower_line, words[i]) != NULL) {
      printf("You entered a prohibited word: %s. Your message will not be sent.\n", words[i]);
      return 1;
    }
  }
  return 0;
}

int main(int ac, char* av[]) {
  if(ac!=2) {
    printf("Usage: %s ttyname\n", av[0]);
  } 
  else {
    int fd = open(av[1], O_WRONLY);
    if(fd == -1) {
      perror("open failed");
    }
    else {
      char *hiMessage="Message from another terminal...";
      char *byeMessage="EOF\n";
      char buf[BUFSIZE];

      write(fd, hiMessage, strlen(hiMessage));
      
      while(fgets(buf, BUFSIZE, stdin)!=0) {
        if(contains_prohibited_word(buf)) {
          continue;
        }

        if(write(fd, buf, strlen(buf))==-1)
        break;
      }
      write(fd, byeMessage, strlen(byeMessage));
      close(fd);
    }
  }
   return 0;
}
