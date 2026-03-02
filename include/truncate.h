/**
 * @file truncate.h
 * @brief Skips whitespace at start of a string.
 * @author Kai Ryall Ota
 * @date March 2026 */
#ifndef TRUNCATE_H_
#define TRUNCATE_H_

/**
 * @brief Removes whitespace at start of a string
 *
 * @param {*char} line   Line to parse.
 *
 * @returns {char*} Pointer to first non-whitespace char in string.
 * */
char *truncate(char *line);

#endif // TRUNCATE_H_
