#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFERSIZE 10240

int main(int ac, char *av[]) {
  int src, dst;

  if(ac != 3) { // ensure there is only 3 parameters
    printf("Usage: %s source destination\n", av[0]);
    return 1;
  }

  src = open(av[1], O_RDONLY);
  if (src<0) { // check if the src file exists
    perror(av[1]);
    return 1;
  }
  dst = creat(av[2], S_IRUSR | S_IWUSR);
  if (dst<0) { // will trigger if creat fails. i.e. directory doesnt exist 
    perror(av[2]);
    return 1;
  }

  char buffer[BUFFERSIZE];
  int numBytes;
  int iteration = 1;

  while((numBytes=read(src, buffer, BUFFERSIZE))>0) {
    if(write(dst,buffer,numBytes) != numBytes) {
      perror("Write error");
      return 1;
    };
    write(dst, buffer, numBytes);
    printf("Iteration: %d, bytes read %d\n", iteration, numBytes);
    iteration++;
  }
  close(src);
  close(dst);
  
  return 0;
}
