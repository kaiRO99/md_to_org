#ifndef REPLACE_ALL_H_
#define REPLACE_ALL_H_
/**
 * @brief Replaces all occurence of a substring.
 * @param {char*} src Original string to parse and edit.
 * @param {char*} old_substring Substring to find and replace.
 * @param {char*} new_substring Substring to insert.
 *
 * @return {char*} edited  New string with replacement.
 * */
char *replace_all(const char *src, const char *old_str, const char *new_str);

#endif // REPLACE_ALL_H_
