/**
 * @file convert_single.h
 * @brief Converts a single markdown file to Org-mode
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#ifndef CONVERT_SINGLE_H_
#define CONVERT_SINGLE_H_

#include "config.h"

/**
 * @brief Convert a single markdown file to Org-mode.
 * @param{char*} source Markdown file to parse.
 * @param {char*} target Org-mode file to write to.
 * @param {Config*} config Configuration to execute parsing.
 * @return{int} Completion status
 * */
int convert_single(const char *source, const char *target, Config *config);

#endif // CONVERT_SINGLE_H_
