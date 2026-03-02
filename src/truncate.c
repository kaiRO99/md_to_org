/**
 * @file truncate.c
 * @brief Removes whitespace at start of a string.
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#include "truncate.h"
#include <stdbool.h>

/**
 * @brief Removes whitespace at start of a string
 *
 * @param {*char} line   Line to parse.
 *
 * @returns {char*} Pointer to first non-whitespace char in string.
 * */
char *truncate(char *line) {
    char *pos;
    pos = line;
    // Remove whitespace

    while (*pos == ' ' || *pos == '\t') {
        pos++;
    }
    return pos;
} // truncate
