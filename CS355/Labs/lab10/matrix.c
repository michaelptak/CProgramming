#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

#define MIN_SPEED 1
#define MAX_SPEED 99

int speed = 1;

void cleanup(int signum) {
  endwin();
  exit(0);
}

char rand_char() {
  int r = rand() % 10;
  if (r < 2) return ' '; // 20% chance of a space
  if (r % 2 == 0) return '0';
  return '1';
}

int main(int ac, char *av[]) {
  if (ac == 2) {
    int arg_speed = atoi(av[1]);
    if (arg_speed >= MIN_SPEED && arg_speed <= MAX_SPEED) {
      speed = arg_speed;
    } else {
      printf("Usage: %s [1-99]\n", av[0]);
      return 1;
    }
  }

  initscr();
  noecho();
  curs_set(FALSE);
  nodelay(stdscr, TRUE);
  srand(time(NULL));
  signal(SIGINT, cleanup);

  // allocate screen buffer
  char **buffer = malloc(LINES * sizeof(char *));
  for (int i = 0; i < LINES; i++) {
    buffer[i] = calloc(COLS + 1, sizeof(char)); // +1 for null-terminator
    for (int j = 0; j < COLS; j++) {
      buffer[i][j] = ' ';
    }
  }

  while (1) {
    // shift rows down
    for (int i = LINES - 1; i > 0; i--) {
      for (int j = 0; j < COLS; j++) {
        buffer[i][j] = buffer[i - 1][j];
      }
    }
    // generate new top row
    for (int j = 0; j < COLS; j++) {
      buffer[0][j] = rand_char();
    }
    // draw buffer
    clear();
    for (int i = 0; i < LINES; i++) {
      mvaddnstr(i, 0, buffer[i], COLS);
    }
    refresh();
    usleep(1000000 / speed);
  }

  return 0;
}
