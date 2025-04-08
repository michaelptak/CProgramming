// A demonstration of using signal()

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void ouch(int); //forward declaration of signal handler 

int ouch_count=0; //count ctrl-c presses 

//prints a message and puts the process to sleep for 1 second over 10 iterations
void long_loop() {
  for(int i=0;i<10; i++) {
    printf("Hello %d\n", i);
    sleep(1);
  }
}

int main(){
  signal(SIGINT, ouch); // reprograms the interrupt system handler
  long_loop();
  return 0;
}

//interrupt signal handler
void ouch(int signum) {
  printf("---OUCH!!!\n");
  ouch_count++;
  if(ouch_count>=3) {
    printf("You are killing me...\n");
    signal(SIGINT, SIG_DFL); // restore deault interrupt signal handler
    raise(SIGINT); // generates an interrupt signal, which kills the process
  }
}
