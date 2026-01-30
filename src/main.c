#include "parse_file.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    // TODO: change this to use
    // TODO: ensure argv[1] is a .md file
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>, <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    parse_file(argv[1], argv[2]);

    exit(0);
}
/**
 * @brief Writes a line to the output file.
 *
 * @param */
// void write_line() {}
