/**
 * @file parse_file.c
 * @brief Parses markdown file line by line and converts to Org-mode.
 * @author Kai Ryall Ota
 * @date Feb 2026
 * */
#include "parse_file.h"
#include "parse_line.h"
#include "replace_substring.h"
#include "starts_with.h"
#include "states.h"
#include "truncate.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Parses a mardown file and converts it to Org mode.
 *
 * @param {*char} filename Filename of markdown file to convert.
 * @param {*char} target_filename Filename of org mode to output to.
 * @return{int} Status of completion
 *            { 0:success,
 *              1:error opening source,
 *              2:error opening target
 *              }
 **/
int parse_file(const char *filename, const char *target_filename) {

    FILE *fin = fopen(filename, "r");         // Input file pointer
    FILE *fout = fopen(target_filename, "w"); // Output file pointer
    char *result;
    // States
    /* HeaderState header_state = H0; */
    SectionState prop_state = MISSING;
    SectionState toc_state = MISSING;
    SubState curr_sub_state = NONE;
    State curr_state = DEFAULT;

    // File pointer validation
    if (fin == NULL) {
        printf("Error: Could not open file.\n");
        return 1;
    } // if
    if (fout == NULL) {
        printf("Error: Could not open file.\n");
        return 2;
    } // if

    char *curr_line = (char *)malloc(sizeof(char) * 1024); // Current line

    while (fgets(curr_line, 1024, fin)) {
        if (curr_state == DEFAULT) {
            //
            if (prop_state == MISSING) {
                if (starts_with(curr_line, "---") == 0) {
                    // Properties
                    curr_state = PROPERTIES;
                    fputs(":PROPERTIES:\n", fout);
                    continue;
                } // if
            } // if

            if (starts_with(curr_line, "```") == 0) {
                // Code block

                curr_state = CODE_BLOCK;
                char *new_line =
                    replace_substring(curr_line, "```", "#+BEGIN_SRC");
                fputs(new_line, fout);
                free(new_line);
                continue;

            } // if

            if (starts_with(curr_line, "#") == 0) {
                // Header
                char *pos = curr_line;
                int counter = 0;
                while (*pos == '#') {
                    *pos = '*';
                    (pos)++;
                    counter++;
                } // while

                // If no table of contents inserted and we are at H2
                if (toc_state == MISSING && counter == 2) {
                    // Insert table of contents before first H2
                    fputs("\n** Table of Contents :TOC:\n\n", fout);
                    toc_state = DONE;
                } // if

                fputs(curr_line, fout);
                continue;
            } // if

            if (starts_with(curr_line, ">") == 0) {
                // Quote
                curr_state = QUOTE;
                fputs("#+BEGIN_QUOTE\n", fout);

                // Remove >
                char *new_line = replace_substring(curr_line, ">", "");

                // NOTE: rather than replace, move curr line to > +1?
                result = parse_line(new_line);
                fputs(result, fout);
                free(new_line);
                free(result);
                result = NULL;
                continue;
            } // if
            result = parse_line(curr_line);
            fputs(result, fout);
            free(result);
            result = NULL;
            continue;

        } else if (curr_state == PROPERTIES) {
            // Properties
            if (starts_with(curr_line, "---") == 0) {
                // End of properties
                curr_state = DEFAULT;
                fputs(":END:\n", fout);
                prop_state = DONE;
                continue;
            } // if
            if (starts_with(curr_line, "- ") == 0) {
                // In list
                fseek(fout, -1, SEEK_CUR);
                if (curr_sub_state != PROP_LIST) {
                    // First property list item
                    curr_sub_state = PROP_LIST;
                    fputc(' ', fout); // for padding
                } // if
                // Not first property list item
                char *new_line = replace_substring(curr_line, "- ", ":");
                char *list_item = truncate(new_line);
                fputs(list_item, fout);
                free(new_line);
                continue;
            } else if (curr_sub_state == PROP_LIST) {
                // End of list
                fseek(fout, -1, SEEK_CUR);
                fputs(":\n", fout);
                curr_sub_state = NONE;

            } // else if

            // Not in a property list
            fputc(':', fout);
            fputs(curr_line, fout);
            continue;
        } else if (curr_state == CODE_BLOCK) {
            if (starts_with(curr_line, "```") == 0) {
                // End of code block
                curr_state = DEFAULT;
                char *new_line =
                    replace_substring(curr_line, "```", "#+END_SRC");
                fputs(new_line, fout);
                free(new_line);
                continue;
            } // if
            // In code block
            fputs(curr_line, fout);
            continue;
        } else if (curr_state == QUOTE) {
            if (starts_with(curr_line, ">") == 0) {
                char *new_line = replace_substring(curr_line, ">", "");

                // NOTE: rather than replace, move curr line to > +1?
                result = parse_line(new_line);
                fputs(result, fout);
                free(result);
                free(new_line);
                result = NULL;
                continue;
            } else {
                // End of blockquote
                curr_state = DEFAULT;
                fputs("#+END_QUOTE\n", fout);

                result = parse_line(curr_line);
                fputs(result, fout);
                free(result);
                result = NULL;
                continue;
            } // if else
        } // else if
    } // while

    if (curr_line) {
        free(curr_line);
    }
    fclose(fin);
    fclose(fout);
    return 0;
} // parse_file
