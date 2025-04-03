#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define BUFSIZE 1024

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
      char buf[BUFSIZE];
      write(fd, hiMessage, strlen(hiMessage));
      while(1){
        fgets(buf, BUFSIZE, stdin);
        write(fd, buf, strlen(buf));
      }
      close(fd);
    }
  }
   return 0;
}
