#include "replace_substring.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Replaces first instance of a substring with another substring.
 *
 * @param {char*} src Original string to parse and edit.
 * @param {char*} old_substring Substring to find and replace.
 * @param {char*} new_substring Substring to insert.
 *
 * @return {char*} edited  New string with replacement. */
char *replace_substring(const char *src, const char *old_substring,
                        const char *new_substring) {
    char *pos = strstr(src, old_substring);
    if (!pos) {
        return strdup(src);
    }

    // Get sizes
    size_t old_len = strlen(old_substring);
    size_t new_len = strlen(new_substring);
    size_t pre_len =
        pos -
        src; // pointer substraction (returns number of characters between them)
    size_t post_len = strlen(pos + old_len);

    char *edited = malloc(pre_len + new_len + post_len + 1);
    if (!edited) {
        return NULL;
    }

    memcpy(edited, src, pre_len); // Copy before the substring
    memcpy(edited + pre_len, new_substring, new_len);
    memcpy(edited + pre_len + new_len, pos + old_len, post_len + 1);

    return edited
        }
