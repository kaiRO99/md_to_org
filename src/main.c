/**
 * @file main.c
 * @brief Entry point for md_to_org
 * @author Kai Ryall Ota
 * @date Feb 2026
 * */
#include "parse_file.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Entry point for md_to_org
 * @param{int} argc number of arguments passed.
 * @param{char*} argv argv[1]: markdown file, argv[2]: target file
 * */
int main(int argc, char *argv[]) {
    // TODO: ensure argv[1] is a .md file
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>, <filename>\n", argv[0]);
        return EXIT_FAILURE;
    } // if
    int result = parse_file(argv[1], argv[2]);

    if (result != 0) {
        fprintf(stderr, "Error opening or parsing files");
    } // if
    exit(0);
} // main()
