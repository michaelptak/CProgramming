#include <curses.h>

int main() {
  initscr();
  clear();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  addstr("Press any key...");
  while(1){
    int ch = getch();
    move(0,0);
    switch (ch) {
      case KEY_UP:
        addstr("UP                                                      ");
        break;
      case KEY_DOWN:
        addstr("DOWN                                                     ");
        break;
      case KEY_LEFT:
        addstr("LEFT                                                     ");
        break;
      case KEY_RIGHT:
        addstr("RIGHT                                                    ");
        break;
      case 'x':
        endwin();
        return 0;
      default:
        printw("You pressed a \"%c\" key, ASCII code %d...", ch, ch);
    }
    refresh();
  }

  endwin();
  return 0;
}
