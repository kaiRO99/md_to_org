/**
 * @file replace_substring.h
 * @brief Replaces first instance of a substring with another.
 * @author Kai Ryall Ota
 * @date Feb 2026
 * */
#ifndef REPLACE_SUBSTRING_H_
#define REPLACE_SUBSTRING_H_

#include <stdlib.h>
#include <string.h>

/**
 * @brief Replaces first instance of a substring with another substring.
 *
 * @param {char*} src Original string to parse and edit.
 * @param {char*} old_substring Substring to find and replace.
 * @param {char*} new_substring Substring to insert.
 *
 * @return {char*} edited  New string with replacement.
 * */
char *replace_substring(const char *src, const char *old_substring,
                        const char *new_substring);

#endif // REPLACE_SUBSTRING_H_
