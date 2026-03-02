/**
 * @file starts_with.h
 * @brief Checks if string starts with a defined string.
 * @author Kai Ryall Ota
 * @date Feb 2026
 * */
#ifndef STARTS_WITH_H_
#define STARTS_WITH_H_

#include <stdbool.h>
/**
 * @brief  Checks if the line starts with a string.
 *
 * @param {*char} line   Line to parse.
 * @param {char[]} key   String to check
 *
 * @returns {bool}
 * */
bool starts_with(char *line, char key[]);

#endif // STARTS_WITH_H_
