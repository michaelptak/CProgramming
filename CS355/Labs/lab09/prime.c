#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

volatile unsigned long largest_prime = 2;

int is_prime(unsigned long n) {
  if (n < 2) return 0;
  if (n == 2) return 1;
  if (n % 2 == 0) return 0;

  for (unsigned long i = 3; i * i <= n; i += 2) {
    if (n % i == 0)
      return 0;
  }
  return 1;
}

char getch() {
  struct termios oldt, newt;
  char ch;
  // Current terminal settings
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  // Disable canonical mode and echo, then apply settings
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  
  read(STDIN_FILENO, &ch, 1);
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  return ch;
}

void handle_sig(int signum) {
  printf("%lu   Quit [y/n]? ", largest_prime);
  fflush(stdout);
  
  char c = getch();
  printf("%c\n", c);
  if (c == 'y' || c == 'Y') {
    printf("\n");
    exit(0);
  }
}

int main() {
  // disable ctrl+c echoing
  struct termios t;
  tcgetattr(STDIN_FILENO, &t);
  t.c_lflag &= ~ECHOCTL;
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
  signal(SIGINT, handle_sig);

  unsigned long num = 3;
  while (1) {
    if (is_prime(num)) {
      largest_prime = num;
    }
    num++;
  }
  return 0;
}
