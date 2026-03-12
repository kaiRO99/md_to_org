/**
 * @file parse_file.h
 * @brief Parses markdown file line by line and converts to Org-mode.
 * @author Kai Ryall Ota
 * @date Feb 2026
 * */

#ifndef PARSE_FILE_H_
#define PARSE_FILE_H_

#include "config.h"

/**
 * @brief Parses a mardown file and converts it to Org mode.
 *
 * @param {*char} filename Filename of markdown file to convert.
 * @param {*char} target_filename Filename of org mode to output to.
 * @param {*Config} Configuration to use while parsing
 * @return{int} Status of completion
 *            { 0:success,
 *              1:error opening source,
 *              2:error opening target
 *              }
 **/
int parse_file(const char *filename, const char *target_filename,
               Config *config);

#endif // PARSE_FILE_H_
