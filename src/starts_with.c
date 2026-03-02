/**
 * @file starts_with.c
 * @brief Checks if string starts with a defined string.
 * @author Kai Ryall Ota
 * @date Feb 2026
 * */
#include "starts_with.h"
#include <stdbool.h>
#include <string.h>

/**
 * @brief  Checks if the line starts with a string.
 *
 * @param {*char} line   Line to parse.
 * @param {char[]} key   String to check
 *
 * @returns {bool}
 * */
bool starts_with(char *line, char key[]) {
    char *temp;
    temp = line;
    // Remove whitespace

    while (*temp == ' ' || *temp == '\t') {
        temp++;
    }
    return strncmp(key, temp, strlen(key));
}
