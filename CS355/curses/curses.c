#include <curses.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int ac, char **av) {
  int speed = 1;
  if (ac == 2){
    speed = atoi(av[1]);
  }


  initscr();

  printf("The screen is %d rows by %d columns", LINES, COLS);
  getchar();
  clear();
  for (int i = 0; i < LINES; i++){
    move(i,i*3);
    addstr("Hello World");
    refresh();
    usleep(500000/speed);
    move(i,i*3);
    addstr("            ");
  }
  getchar();
  endwin();

  return 0;
}
