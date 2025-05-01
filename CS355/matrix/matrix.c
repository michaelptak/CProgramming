#include <stdlib.h>
#include <curses.h>
#include <unistd.h>

int main() {
  initscr();
  curs_set(FALSE);
  int count = 1;
  while(1){
    int r = rand() % 3;
    if(r==0) addstr(" ");
    if(r==1) addstr("0");
    if(r==2) addstr("1");
    count++;
    if(count>=COLS){
      refresh();
      usleep(500000);
      count = 1;
      move(0,0);
      insertln();
    }
  }
  getchar();
  endwin();
}
