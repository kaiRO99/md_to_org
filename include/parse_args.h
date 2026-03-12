/**
 * @file parse_args.h
 * @brief Parses command line arguments.
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#ifndef PARSE_ARGS_H_
#define PARSE_ARGS_H_

#include "config.h"

/**
 * @brief Parses command line arguments.
 * @param{int} argc Number of command line arguments.
 * @param{char*} argv Array of command line arguments.
 * @param {Config*} config Configuration object to parse into.
 * @return {int} Staus of parse: 0:complete, 1: error, -2: --help flag
 * */
int parse_args(int argc, char *argv[], Config *config);

#endif // PARSE_ARGS_H_
