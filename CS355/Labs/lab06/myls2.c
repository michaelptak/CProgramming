#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define MAX_ENTRIES 1000
#define MAX_NAME_LENGTH 256

// Comparison functions for qsort
int compare_lexicographic(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int compare_reverse_lexicographic(const void *a, const void *b) {
    return strcmp(*(const char **)b, *(const char **)a);
}

//Function to print entryies in columns
void print_entries_in_columns(char **entries, int count) {
  struct winsize wbuf;
  int max_len = 0;

  //get terminal width
  if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &wbuf) == 1) {
    perror("ioctl");
    exit(1);
  }

  //determine longest filename
  for (int i =0; i<count; i++) {
    int len = strlen(entries[i]);
    if(len > max_len) {
      max_len = len;
    }
  }

  max_len += 2; // spacing between columns
  int cols = wbuf.ws_col / max_len;
  if (cols == 0) {
    cols = 1;
  }
  int rows = (count + cols -1) / cols;

  for (int row = 0; row < rows; row++) {
    for (int col = 0; col <cols; col++) {
      int index = col * rows + row;
      if (index < count) {
        printf("%-*s", max_len, entries[index]);
      }
    }
    printf("\n");
  }
}


void do_ls(char *dirname, int show_hidden, int sort_type) {
    DIR *dir;
    struct dirent *entry;
    char *entries[MAX_ENTRIES];
    int entry_count = 0;

    // Open directory
    dir = opendir(dirname);
    if (dir == NULL) {
        perror("Error opening directory");
        exit(1);
    }

    // Read directory entries
    while ((entry = readdir(dir)) != NULL && entry_count < MAX_ENTRIES) {
        // Skip hidden entries if not showing all
        if (!show_hidden && entry->d_name[0] == '.') {
            continue;
        }

        // Allocate memory and copy entry name
        entries[entry_count] = malloc(strlen(entry->d_name) + 1);
        strcpy(entries[entry_count], entry->d_name);
        entry_count++;
    }

    // Sort entries if needed
    if (sort_type == 's') {
        qsort(entries, entry_count, sizeof(char *), compare_lexicographic);
    } else if (sort_type == 'r') {
        qsort(entries, entry_count, sizeof(char *), compare_reverse_lexicographic);
    }

    print_entries_in_columns(entries, entry_count);

    // free memory
    for (int i = 0; i < entry_count; i++) {
        free(entries[i]);
    }

    // Close directory
    closedir(dir);
}

int main(int ac, char *av[]) {
    int opt;
    int show_hidden = 0;
    int sort_type = 0;
    char *directory = ".";

    // Parse command line options
    while ((opt = getopt(ac, av, "ars")) != -1) {
        switch (opt) {
            case 'a':
                show_hidden = 1;
                break;
            case 'r':
                sort_type = 'r';
                break;
            case 's':
                sort_type = 's';
                break;
            default:
                fprintf(stderr, "Usage: %s [-ars] [directory]\n", av[0]);
                exit(1);
        }
    }

    // Check if a directory is specified after options
    if (optind < ac) {
        directory = av[optind];
    }

    // Call do_ls with specified options
    do_ls(directory, show_hidden, sort_type);

    return 0;
}
