#include "wordcount.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char** argv) {
    // Argument Handling
    if (argc != 2) {
        perror("This program needs exactly 1 argument.");
        return -1;
    }

    if (argv[1] == NULL) {
        perror("File does not meet compatibility requirements or does not exist.");
        return -1;
    }

    // Open File
    FILE* words = fopen(argv[1], "r");
    const char* filename = argv[1];

    struct stat sb;
    long total_bytes;

    // Use stat to return total bytes in file
    if (stat(filename, &sb) == -1) {
        perror("Stat Error");
        return -1;
    }
    else {
        total_bytes = sb.st_size;
    }

    long total_words = wordcount(words, total_bytes);

    printf("\nThere are %ld words in %s.\n", total_words, filename);

    fclose(words);
    return 1;
}