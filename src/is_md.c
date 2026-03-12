/**
 * @file is_md.c
 * @brief Checks if file extension is .md or .markdown
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#include "is_md.h"
#include <string.h>

/**
 * @brief Checks if file has .md extension
 * @param{char*} filename Filename to check.
 * @return{int} Status of check
 * */
int is_md(const char *filename) {
    if (!filename)
        return 0;

    size_t len = strlen(filename);

    // check for .md
    if (len < 4)
        return 0;

    // Check for .md (case insensitive)
    const char *extension = filename + len - 3;
    if (strcmp(extension, ".md") == 0) {
        return 1;
    } // if

    // check for .makrdown
    if (len >= 10 && strcmp(filename + len - 9, ".markdown") == 0) {
        return 1;
    } // if
    return 0;
} // is_md()
