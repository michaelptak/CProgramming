#include <stdio.h>

// Function to check if a character is a letter (A-Z or a-z)
int is_letter(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

int main(int argc, char *argv[]) {
    // Loop through each argument
    for (int i = 1; i < argc; i++) {
        char filtered[100]; // Temporary array to store filtered characters
        int len = 0;
        
        // Filter only letters from the argument
        for (int j = 0; argv[i][j] != '\0'; j++) {
            if (is_letter(argv[i][j])) {
                filtered[len++] = argv[i][j];
            }
        }
        
        // Print the filtered string progressively
        for (int k = 0; k < len; k++) {
            for (int m = 0; m <= k; m++) {
                putchar(filtered[m]);
            }
            putchar('\n');
        }
    }
    
    return 0;
}
