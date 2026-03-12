/**
 * @file convert_single.c
 * @brief Converts a single markdown file to Org-mode
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#include "convert_single.h"
#include "config.h"
#include "parse_file.h"
#include <stdio.h>

/**
 * @brief Convert a single markdown file to Org-mode.
 * @param{char*} source Markdown file to parse.
 * @param {char*} target Org-mode file to write to.
 * @param {Config*} config Configuration to execute parsing.
 * @return{int} Completion status
 * */
int convert_single(const char *source, const char *target, Config *config) {
    // pass config to parse_file()
    if (config->dry_run) {
        printf("[DRY RUN] Convert: %s -> %s\n", source, target);
        return 0;
    } // if

    if (!config->force) {
        FILE *check = fopen(target, "r");
        if (check) {
            fclose(check);
            fprintf(stderr, "Error: %s already exists. Use -f to overwrite.\n",
                    target);
            return 1;
        } // if
    }
    // parse file
    int result = parse_file(source, target, config);
    if (result != 0) {
        // TODO: error handling
        fprintf(stderr, "Error converting %s", source);
        return result;
    } // if
    return 0;
} // convert_single()
