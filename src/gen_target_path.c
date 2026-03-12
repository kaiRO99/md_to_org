/**
 * @file gen_target_path.c
 * @brief Generate the target path based on input path.
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#define _POSIX_C_SOURCE 200809L
#include "gen_target_path.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Generate output file in same directory as input file.(".md"->".org")
 * @param{*char} input_path
 * @return{char*} Filepath of target file.
 * */
char *generate_target_path(const char *input_path) {
    if (!input_path) {
        fprintf(stderr, "ERROR: input_path is NULL in generate_output_name\n");
        return NULL;
    }
    char *input_copy = strdup(input_path);
    // char *base = basename(input_copy); // if need to extract just filename
    // NOTE: use in is_md()? <sys/syay.h>?

    // remove .md
    char *dot = strrchr(input_copy, '.');
    if (dot && strcmp(dot, ".md") == 0) {
        *dot = '\0';
    } // if

    // build outout path: same location, cjange extension
    char *output = malloc(strlen(input_copy) + 5);
    sprintf(output, "%s.org", input_copy);

    free(input_copy);
    return output;
} // generate_target_path()
