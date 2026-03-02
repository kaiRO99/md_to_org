/**
 * @file replace_all.c
 * @brief Replaces all instances of a substring with another.
 * @author Kai Ryall Ota
 * @date Feb 2026
 * */
#define _POSIX_C_SOURCE 200809L
#include "replace_all.h"
#include <stdlib.h>
#include <string.h>

/**
 * @brief Replaces all occurence of a substring.
 * @param {char*} src Original string to parse and edit.
 * @param {char*} old_substring Substring to find and replace.
 * @param {char*} new_substring Substring to insert.
 *
 * @return {char*} edited  New string with replacement. */
char *replace_all(const char *src, const char *old_str, const char *new_str) {

    if (!src || !old_str || !new_str) {
        return NULL;
    } // if

    const char *pos = src;
    size_t old_len = strlen(old_str);
    size_t new_len = strlen(new_str);

    int count = 0;

    // Count occurences of old to allocate mem for new string
    while (((pos = strstr(pos, old_str)) != NULL)) {
        // pos is the position of a match
        count++;
        pos += old_len;
    } // while
    if (count == 0) {
        return strdup(src);
    } // if

    // new string
    size_t new_size = strlen(src) + count * (new_len - old_len) + 1;
    char *result = malloc(new_size);
    if (!result) {
        return NULL;
    } // if
    char *replaced = result;
    const char *temp = src;
    // replace all occurences
    while (*temp) {
        const char *match = strstr(temp, old_str);
        if (match) {
            // copy pre
            size_t prefix_len = match - temp;
            memcpy(replaced, temp, prefix_len);
            replaced += prefix_len;

            // copy replacement
            memcpy(replaced, new_str, new_len);
            replaced += new_len;

            // move temp
            temp = match + old_len;
        } else {
            // copy post
            strcpy(replaced, temp);
            break;
        }
    }
    strcpy(replaced, temp);

    return result;
} // replace_all
