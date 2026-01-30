#include "parse_file.h"
#include <stdbool.h>
#include <string.h>
/* #include <stddef.h> */
#include <stdio.h>
#include <stdlib.h>

bool in_code_block = false;
bool in_code = false;
bool in_bold = false;
bool in_italic = false;
bool in_sthrough = false;

/**
 * @brief
 *
 * @param
 * @return
 * */
void parse_file(const char *filename, const char *target_filename) {
    // file pointer
    FILE *fin = fopen(filename, "r");
    FILE *fout = fopen(target_filename, "w");
    if (fin == NULL) {
        printf("Error: Could not open file.\n");
        exit(EXIT_FAILURE);
    }
    if (fout == NULL) {
        printf("Error: Could not open file.\n");
        exit(EXIT_FAILURE);
    }
    // pointer for current line it is processing
    char *curr_line = (char *)malloc(sizeof(char) * 1024); // TODO: verify size
    // pointer for when some string concatenation is required
    char *new_str = (char *)malloc(sizeof(char) * 1024); // TODO: verify size
    char *temp;
    /* char *original_line;*/
    /* char *output_line; */
    int curr_char;
    int prev = '\n';

    while ((curr_char = fgetc(fin)) != EOF) {
        /* char *original_line = curr_line; */
        /* char *output_line = curr_line; */

        // TODO: check if temp is a blank line or eof,
        // TODO: if else on major syntax checkers
        // TODO: if in block, don't do any of this
        if (curr_char == '\n') {
            // Empty line
            prev = '\n';
            fputc(curr_char, fout);
            continue;
        } else if ((curr_char == '#') && (!in_code_block) &&
                   (prev == '\n' || prev == '\0')) {
            // Header
            ungetc(curr_char, fin);
            fgets(curr_line, 1024, fin);
            temp = curr_line;
            while (*temp == '#') {
                *temp = '*';
                (temp)++;
            }
            fputs(curr_line, fout);
            prev = '\n';
            continue;
        }
        // NOTE: Codeblock
        // TODO: fix
        // ``` language
        // ```
        // into
        // #+BEGIN_SRC language
        // #+END_SRC
        // need to start
        if ((curr_char == '`') && (prev == '\n' || prev == '\0')) {
            // check next 2, put in minibuffer
            char buffer[4];
            buffer[0] = (char)curr_char;
            for (int i = 1; i < 3; i++) {
                int c = fgetc(fin);
                if (c == EOF) {
                    break;
                }
                buffer[i] = (char)c;
            }

            // if buffer contains ''', we have a codeblock, if in_code_block is
            // false, we are in first line, if true, we want second.

            if (strcmp(buffer, "```") == 0) {
                // it is a codeblock
                // put the buffer back
                for (int i = 2; i >= 0; i--) {
                    ungetc(buffer[i], fin);
                }
                // then get the full line and process
                fgets(curr_line, 1024, fin);
                if (in_code_block == false) {

                    char *curr_pos = new_str;
                    // copy #+BEGIN_SRC
                    strncpy(curr_pos, "#+BEGIN_SRC ", 12);
                    curr_pos += 12;
                    // ! Is there a better way to do this? no need for curr pos
                    // or new str, strcpy directly into new line?

                    // copy rest of string
                    strcpy(curr_pos, curr_line + 3);
                    // set curr_line to new str
                    strcpy(curr_line, new_str);
                    in_code_block = true;

                    fputs(curr_line, fout);
                    prev = '\n';
                    continue;
                } else {
                    // we are at end of block
                    // need to also print the \n
                    char new_line[] = "#+END_SRC\n";
                    strcpy(curr_line, new_line);
                    in_code_block = false;
                    fputs(curr_line, fout);
                    prev = '\n';
                    continue;
                }
            }
        } else if (curr_char == '`') {
            // we have a normal code?
            // want a single ~
            // When in this, we don't want to use the other rules
            in_code = !in_code;

            fputc('~', fout);
            prev = '~';
            continue;
        }
        if (!in_code && !in_code_block) {
            // check for emphasis
            //  if * or _ we need to check the next too
            if (curr_char == '*' || curr_char == '_') {
                // ensure next is the same
                // also check if prev is space
                // this could mess up some random * char that are not for bold
                int next_char = fgetc(fin);
                if (next_char == curr_char) {
                    // Bold
                    if ((prev != ' ' || prev != '\n' || prev != '\t' ||
                         prev != '\0' || prev != '\f') &&
                        !in_bold) {
                        // we want space followed by *
                        fputc(' ', fout);
                        fputc('*', fout);
                        in_bold = !in_bold;
                    } else if (in_bold) {
                        // we want * followed by a space
                        fputc('*', fout);
                        fputc(' ', fout);
                        in_bold = !in_bold;
                    }
                    // we have "consumed" both bold chars so we can simply move
                    // on

                    prev = next_char;
                    continue;
                } else {
                    // Italics
                    // put back next_char

                    ungetc(next_char, fin);
                    if ((prev != ' ' || prev != '\n' || prev != '\t' ||
                         prev != '\0' || prev != '\f') &&
                        !in_italic) {
                        // we want a space followed by /
                        fputc(' ', fout);
                        fputc('/', fout);
                        in_italic = !in_italic;
                    } else {
                        fputc('/', fout);
                        fputc(' ', fout);
                        in_italic = !in_italic;
                    }
                    prev = curr_char;
                    continue;
                }
            } else if (curr_char == '~') {
                // check second char

                int next_char = fgetc(fin);
                if (next_char == curr_char) {
                    // strikethrough
                    if ((prev != ' ' || prev != '\n' || prev != '\t' ||
                         prev != '\0' || prev != '\f') &&
                        !in_sthrough) {
                        // we want space followed by +
                        fputc(' ', fout);
                        fputc('+', fout);
                        in_sthrough = !in_sthrough;
                    } else if (in_sthrough) {
                        // we want * followed by a space
                        fputc('+', fout);
                        fputc(' ', fout);
                        in_sthrough = !in_sthrough;
                    }
                    // we have "consumed" both bold chars so we can simply move
                    // on
                    prev = next_char;
                    continue;
                } else {
                    // Italics
                    // put back next_char

                    ungetc(next_char, fin);
                    fputc(curr_char, fout);
                    prev = curr_char;
                    continue; // go to next char
                }
            }
        }
        // unordered and ordered lists are the same
        // checkboxes are the same

        // I need to handle multiline emphasis and combined emphasis.
        // Global for each, If I come across an emphasis char and any is set to
        // true, then it is the second one.
        // TODO: bold
        // TODO: Italics
        // TODO: strikethroughs
        // TODO: make this only run if it is paragraph or within a quote?
        // NOTE: ** and __ also trigger italics, but because of the order of the
        // if else it does not trigger
        // TODO put this in a while loop, each step increments temp by the
        // number of symbols to ensure all are caught.

        fputc(curr_char, fout);
        prev = curr_char;
        // print curr char, set prev to curr
    } // while
    // TODO: links - image, url
    // TODO: parameters
    // need to parse entire line
    // TODO: blockquote

    // TODO: highlight - not in org mode, so bold?
    // TODO: tables

    if (curr_line) {
        free(curr_line);
    }
    if (new_str) {
        free(new_str);
    }
    fclose(fout);
    fclose(fin);
}

/**
 * @brief Shifts everything after str back by 1
 * */
void delete_char(char *str) { memmove(str, str + 1, strlen(str)); }
