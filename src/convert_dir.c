/**
 * @file convert_dir.c
 * @brief Converts markdown files in a directory
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#include "convert_dir.h"
#include "config.h"
#include "convert_single.h"
#include "gen_target_path.h"
#include "is_dir.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/**
 * @brief Convert any markdown file in a directory.
 * @param{char*} source_dir Directory to search and convert files from.
 * @param{char*} target_dir Directory to write output files to.
 * @param {Config*} config Configuration to execute parsing.
 * @return{in} Completion status.
 * */
int convert_dir(const char *source_dir, const char *target_dir,
                Config *config) {
    DIR *dir = opendir(source_dir);
    if (!dir) {
        fprintf(stderr, "Error: cannot open directory %s\n", target_dir);
        return 1;
    } // if

    // create output dir
    if (!config->dry_run) {
        mkdir(target_dir, 0755);
    } // if

    struct dirent *entry;
    int error_count = 0;
    int file_count = 0;

    while ((entry = readdir(dir)) != NULL) {
        // skip . and ..
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {
            continue;
        } // if

        // build full input path
        char input_path[1024];
        snprintf(input_path, sizeof(input_path), "%s/%s", source_dir,
                 entry->d_name);

        // check if it is an .md
        size_t len = strlen(entry->d_name);
        if (len > 3 && strcmp(entry->d_name + len - 3, ".md") == 0) {
            // generate target path
            char *target_name = generate_target_path(entry->d_name);
            char target_path[1024];
            snprintf(target_path, sizeof(target_path), "%s/%s", target_dir,
                     target_name);

            // convert
            if (convert_single(input_path, target_path, config) != 0) {
                error_count++;
            } else {
                file_count++;
            } // if else

            free(target_name);
        } // if

        // if recursive
        else if (config->recursive && is_dir(input_path)) {
            char sub_output[1024];
            snprintf(sub_output, sizeof(sub_output), "%s/%s", target_dir,
                     entry->d_name);
            int result = convert_dir(input_path, sub_output, config);
            if (result != 0) {
                error_count++;
            } // if
        } // else if
    } // while
    closedir(dir);
    if (!config->dry_run) {
        printf("Converted %d files from %s\n", file_count, source_dir);
        if (error_count > 0) {
            printf("Failed: %d file(s)\n", error_count);
        } // if
    } // if
    return error_count > 0 ? 1 : 0;
} // convert_dir()
