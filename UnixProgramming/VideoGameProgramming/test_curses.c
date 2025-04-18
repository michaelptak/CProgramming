
#include <curses.h>

int main() {
    initscr();              // initialize screen
    printw("Hello, curses! Press any key to exit.");
    refresh();              // print it on the real screen
    getch();                // wait for user input
    endwin();               // clean up
    return 0;
}
