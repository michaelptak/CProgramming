#include <curses.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define MAX_SNAKE_LENGTH 1024  // safely supports very large terminal sizes
#define MOVE_DELAY 100000      // Base microseconds between moves

typedef struct {
    int y;
    int x;
} Point;

Point snake[MAX_SNAKE_LENGTH];
int snake_length = 3;          // initial snake length of 3
int dx = 0, dy = 0;

// Trophy state
Point trophy;
int trophy_value;
time_t trophy_spawn_time;
int trophy_lifetime;

// Function prototypes
void show_end_screen(const char* msg);

// Set the initial direction randomly to up, down, left, or right
void set_random_direction() {
    int dir = rand() % 4;
    switch (dir) {
        case 0: dx = 1;  dy = 0;  break;  // Right
        case 1: dx = -1; dy = 0;  break;  // Left
        case 2: dx = 0;  dy = 1;  break;  // Down
        case 3: dx = 0;  dy = -1; break;  // Up
    }
}

// Spawn a new trophy at a random location not occupied by the snake
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

// Initialize snake centered in the pit
void init_snake() {
    int head_y = LINES / 2;
    int head_x = COLS / 2 - snake_length / 2;
    for (int i = 0; i < snake_length; i++) {
        snake[i].y = head_y - dy * i;
        snake[i].x = head_x - dx * i;
    }
}

// Draw snake and trophy
void draw_elements() {
    for (int i = 0; i < snake_length; i++) {
        mvaddch(snake[i].y, snake[i].x, (i == 0 ? 'O' : 'o'));
    }
    mvaddch(trophy.y, trophy.x, '0' + trophy_value);
}

// Delay inversely proportional to snake length (longer snake -> faster)
int get_delay() {
    int base = MOVE_DELAY;
    int delay = base - (snake_length * 500);  // 500μs faster per segment
    if (delay < 20000) delay = 20000;
    return delay;
}

// Setup curses and RNG
void init_game() {
    initscr();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    srand(time(NULL));
}

// Restore terminal
void cleanup() {
    nodelay(stdscr, FALSE);
    getch();
    endwin();
}

// Process input; reverse attempt kills the snake
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
        default:
            break;
    }
    return 1;
}

// Move snake forward
void update_snake_position() {
    for (int i = snake_length - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0].x += dx;
    snake[0].y += dy;
}

// Check for collision with wall or self
int check_collision() {
    Point h = snake[0];
    if (h.x <= 0 || h.x >= COLS - 1 || h.y <= 0 || h.y >= LINES - 1)
        return 1;
    for (int i = 1; i < snake_length; i++) {
        if (snake[i].x == h.x && snake[i].y == h.y)
            return 1;
    }
    return 0;
}

// Check win: length >= half the pit perimeter
int check_win() {
    int perim = (COLS - 2) + (LINES - 2);
    return snake_length >= perim;
}

// Display end screen
void show_end_screen(const char* msg) {
    int score = (snake_length - 3) * 10;
    if (score < 0) score = 0;
    erase(); box(stdscr, 0, 0);
    int my = LINES / 2, mx = COLS / 2;
    mvprintw(my - 1, mx - strlen(msg) / 2, "%s", msg);
    char buf[32]; snprintf(buf, sizeof(buf), "Score: %d", score);
    mvprintw(my, mx - strlen(buf) / 2, "%s", buf);
    mvprintw(my + 2, mx - 12, "Press any key to quit.");
    nodelay(stdscr, FALSE);
    refresh(); getch();
}

// Main loop
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

int main(void) {
    init_game();
    set_random_direction();
    init_snake();
    run_game();
    cleanup();
    return 0;
}
