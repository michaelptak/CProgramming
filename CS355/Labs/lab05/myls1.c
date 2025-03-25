#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#define MAX_ENTRIES 1000
#define MAX_NAME_LENGTH 256

// Comparison functions for qsort
int compare_lexicographic(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

int compare_reverse_lexicographic(const void *a, const void *b) {
    return strcmp(*(const char **)b, *(const char **)a);
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

    // Print entries
    for (int i = 0; i < entry_count; i++) {
        printf("%s\n", entries[i]);
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
