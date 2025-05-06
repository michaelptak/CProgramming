#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define MAX_SNAKE_LENGTH 1024  // safely supports very large terminal sizes
#define MOVE_DELAY 100000  // Microseconds between moves. This value is passed to usleep() to pause between frames.

typedef struct {
    int y;
    int x;
} Point;

Point snake[MAX_SNAKE_LENGTH];
int snake_length = 3; // Initial snake length of 3
int dx = 0, dy = 0;

// Trophy state
// Author: Alex Curiale
Point trophy;
int trophy_value;
time_t trophy_spawn_time;
int trophy_lifetime;

void show_end_screen(const char* msg);

// Set the initial direction randomly to up, down, left, right
// Author: Michael Ptak 
void set_random_direction() {
  int dir = rand() % 4;
  switch (dir) {
    case 0: dx = 1; dy = 0;  break;  // Right
    case 1: dx = -1; dy = 0; break;  // Left
    case 2: dx = 0; dy = 1;  break;  // Down
    case 3: dx = 0; dy = -1; break;  // Up  
  }
}

// Spawn a new trophy at a random location not occupied by the snake
// Author: Alex Curiale
void spawn_trophy() {
    trophy_value = rand() % 9 + 1;          // value 1-9
    int y, x, on_snake;
    do {
        y = rand() % (LINES - 2) + 1;
        x = rand() % (COLS - 2) + 1;
        on_snake = 0;
        for (int i = 0; i < snake_length; i++) {
            if (snake[i].y == y && snake[i].x == x) {
                on_snake = 1;
                break;
            }
        }
    } while (on_snake);
    trophy.y = y;
    trophy.x = x;
    trophy_spawn_time = time(NULL);
    trophy_lifetime = rand() % 9 + 1;        // seconds
}

// Initialize snake in the center
// Author: Michael Ptak
void init_snake() {
    int head_y = LINES / 2; // Find center of terminal
    int head_x = COLS / 2;
  
    for (int i = 0; i < snake_length; i++) {
        snake[i].y = head_y - (dy * i);  // move opposite to direction
        snake[i].x = head_x - (dx * i);
    }
}

// Draw snake body on screen
// Author: Michael Ptak
// Modified by Alex (Draw trophies as well)
void draw_elements() {
    for (int i = 0; i < snake_length; i++) {
        mvaddch(snake[i].y, snake[i].x, (i == 0 ? 'O' : 'o')); // If the first index, use O for the head
    }
    mvaddch(trophy.y, trophy.x, '0' + trophy_value);
}


// Calculates delay between moves based on snake length
// Author: Michael Ptak
int get_delay() {
  int base_delay = 100000; // in microseconds
  int scaling = 1500; // how much speed increases per extra length
  int min_delay = 30000; // cap so it doesn't go too fast

  int speedup = (snake_length - 5) * scaling;
  int delay = base_delay - speedup;

  if (delay < min_delay) return min_delay;
  return delay;
}

// Setup curses environment and RNG
// Author: Michael Ptak
void init_game() {
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    srand(time(NULL)); // Seed for true RNG in direction 
}

// Restore terminal to normal
// Author: Michael Ptak
void cleanup() {
    nodelay(stdscr, FALSE);
    getch();
    endwin();
}
 
// Process user input and update direction of the snake movement. Press q to quit. 
// Author: Alex Curiale
// Modified by Michael (added cheat codes)
int handle_input() {
    int ch = getch();
    switch (ch) {
        case 'q':
            return 0;
        case KEY_UP:
            if (dy == 1) { show_end_screen("You lost!"); return 0; }
            if (dy == 0) { dx = 0; dy = -1; }
            break;
        case KEY_DOWN:
            if (dy == -1) { show_end_screen("You lost!"); return 0; }
            if (dy == 0) { dx = 0; dy = 1; }
            break;
        case KEY_LEFT:
            if (dx == 1) { show_end_screen("You lost!"); return 0; }
            if (dx == 0) { dx = -1; dy = 0; }
            break;
        case KEY_RIGHT:
            if (dx == -1) { show_end_screen("You lost!"); return 0; }
            if (dx == 0) { dx = 1; dy = 0; }
            break;
        case 'g':  // Cheat code to test win condition 
            snake_length += 5;
            if (snake_length > MAX_SNAKE_LENGTH)
                snake_length = MAX_SNAKE_LENGTH;
            break;
        default:
            break;
    }
    return 1;
}
// Shifts the head and body segment's coordinates from the segment in front of it, making the snake "slither" forward.
// Author: Alex Curiale
void update_snake_position() {
    for (int i = snake_length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0].x += dx;
    snake[0].y += dy;
}

// Checks if the snake has collided with the wall or itself
// Author: Michael Ptak
int check_collision() {
  Point head = snake[0];

  // Collision with border (edges of the screen)
  if (head.x <= 0 || head.x >= COLS - 1 || head.y <= 0 || head.y >= LINES - 1) {
    return 1;
  }

  // Collision with self
  for (int i = 1; i < snake_length; i++) {
    if (snake[i].x == head.x && snake[i].y == head.y) {
      return 1;
    }
  }
  return 0;
}

// Returns 1 if snake has reached winning length
// Author: Michael Ptak
int check_win() {
  int win_length = (2 * (LINES - 2 + COLS - 2)) / 2;
  return snake_length >= win_length;
}

// Print the win/loss screen to the user 
// Author: Michael Ptak
void show_end_screen(const char* message) {
  int score = (snake_length - 3) * 10; // Snake start with length 5, so any length beyond that is 10 pts.
  if (score < 0) score = 0;

  erase();
  box(stdscr, 0, 0);

  int mid_y = LINES / 2;
  int mid_x = COLS / 2; 
  
  mvprintw(mid_y - 1, mid_x - (int)(strlen(message) / 2), "%s", message); 
  char score_text[32];
  snprintf(score_text, sizeof(score_text), "Score: %d", score);
  mvprintw(mid_y, mid_x - (int)(strlen(score_text) / 2), "%s", score_text);
  mvprintw(mid_y + 2, mid_x - 12, "Press any key to quit.");

  nodelay(stdscr, FALSE); // Make getch blocking again
  refresh();
  getch();
}

// Keeps the game running, reads key inputs, returns false when "q" is pressed. Added trophy functionality
// Author: Alex Curiale
// Modified by Michael (Check collision, win/loss, add speedup)
void run_game() {
    spawn_trophy();
    while (1) {
        if (!handle_input()) break;
        update_snake_position();
        // Eat trophy?
        if (snake[0].x == trophy.x && snake[0].y == trophy.y) {
            snake_length += trophy_value;
            if (snake_length > MAX_SNAKE_LENGTH)
                snake_length = MAX_SNAKE_LENGTH;
            spawn_trophy();
        }
        // Expire trophy?
        if (difftime(time(NULL), trophy_spawn_time) >= trophy_lifetime) {
            spawn_trophy();
        }
        if (check_collision()) { show_end_screen("You lost!"); break; }
        if (check_win())       { show_end_screen("You won!");  break; }
        erase(); box(stdscr, 0, 0);
        draw_elements();
        refresh();
        usleep(get_delay());
    }
}

int main() {
    init_game();
    set_random_direction();
    init_snake();
    run_game();
    cleanup();
    return 0;
}
