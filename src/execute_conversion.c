/**
 * @file execute_conversion.c
 * @Brief Handles execution of conversion logic
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#include "execute_conversion.h"
#include "config.h"
#include "convert_dir.h"
#include "convert_single.h"
#include "gen_target_path.h"
#include "is_dir.h"
#include "is_md.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/**
 * @brief Handles logic of conversion execution
 * @param{Config*} config Configuration object
 * @return{int} Status of completion
 * */
int execute_conversion(Config *config) {
    int generated_path = 0;
    // check if source exists
    struct stat st;
    if (stat(config->source_path, &st) != 0) {
        fprintf(stderr, "Error: Source %s does not exist\n",
                config->source_path);
        return 1;
    } // if

    // check if source is file or dir
    if (is_dir(config->source_path)) {
        // Directory
        if (!config->target_path) {
            // default output directory
            //  TODO: this may be a mistake
            config->target_path = config->source_path;
        } // if
        return convert_dir(config->source_path, config->target_path, config);

    } else {
        // file
        // ensure is markdown file
        if (!is_md(config->source_path)) {
            fprintf(stderr, "Error: Input file must be .md or .markdown\n");
            fprintf(stderr, "Got: %s\n", config->source_path);
            return 1;
        } // if

        if (!config->target_path) {
            // generate target filename
            generated_path = 1;
            config->target_path = generate_target_path(config->source_path);
        } // if

        int result =
            convert_single(config->source_path, config->target_path, config);

        // NOTE: add a check to see if it did use generate_target_path()?
        if (config->target_path && generated_path) {
            free(config->target_path);
        } // if
        return result;
    } // if else

} // execute_conversion()
