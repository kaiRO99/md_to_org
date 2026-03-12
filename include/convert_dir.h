/**
 * @file convert_dir.h
 * @brief Converts markdown files in a directory
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#ifndef CONVERT_DIR_H_
#define CONVERT_DIR_H_

#include "config.h"

/**
 * @brief Convert any markdown file in a directory.
 * @param{char*} source_dir Directory to search and convert files from.
 * @param{char*} target_dir Directory to write output files to.
 * @param {Config*} config Configuration to execute parsing.
 * @return{in} Completion status.
 * */
int convert_dir(const char *source_dir, const char *target_dir, Config *config);

#endif // CONVERT_DIR_H_
