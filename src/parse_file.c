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
bool in_quote = false;
bool in_frontmatter = false;
bool in_frontmatter_list = false;
int h6_counter = 0;
int h5_counter = 0;
int h4_counter = 0;
int h3_counter = 0;
int h2_counter = 0;
int h1_counter = 0;
int curr_header = 0;
int prev_header = 0;

/**
 * @brief
 *
 * @param
 * @return
 * BUG: links may break
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
    int curr_char;
    int prev = '\n';

    // check first char. If it is -, check next 2, if ---, we have properties
    curr_char = fgetc(fin);
    if (curr_char == '-') {
        // we need to check next 2 chars
        int next_char = fgetc(fin);
        int next_next_char = fgetc(fin);
        if (next_char == '-' && next_next_char == '-') {
            // we have yaml frontmatter
            // BUG: A file that starts with line --- but no frontmatter may
            // NOTE: Do I want to use :PROPERTIES: and #+keywords?- for now, use
            // :PROPERTIES:
            // NOTE: may want have #+ keywords for title, tags, and
            // related
            // NOTE: if It has a "class", add a tag for school cause
            // TODO: make tagnames uppercase? - not case sensitive so whatever
            in_frontmatter = !in_frontmatter;
            // need to handle lists and likns as well
            fputs(":PROPERTIES:\n", fout);

            // discard rest of line
            fgets(curr_line, 1024, fin);

            // get each line
            while (strstr(fgets(curr_line, 1024, fin), "---") == NULL) {
                // we are not at end of frontmatter
                temp = curr_line;
                // check for a list
                // skip all whitespace
                // BUG: will this skip whitespace in tags?
                while (*temp == ' ' || *temp == '\t') {
                    prev = *temp;
                    temp++;
                }
                /* if (prev == '\n') { */
                /*     fputc(':', fout); */
                /* } */

                // temp should be at first non-space char
                if (*temp == '-' && !in_frontmatter_list) {
                    // first list item

                    // we have a list
                    in_frontmatter_list = !in_frontmatter_list;
                    fseek(fout, -1,
                          SEEK_CUR); // we want to append to previous line
                    fputs(" :", fout);
                    prev = ' ';
                    temp = temp + 2; // skip space after -
                    while (*temp != '\n') {
                        // copy over
                        fputc(*temp, fout);
                        prev = *temp;
                        temp++;
                        continue;
                    }

                    // NOTE: may need to add : after
                    continue;
                } else if (*temp == '-' && in_frontmatter_list == true) {
                    // we have a list item that is not the first
                    fputs(":", fout);
                    prev = ' ';
                    temp = temp + 2; // skip space after -
                    while (*temp != '\n') {
                        // copy over
                        fputc(*temp, fout);
                        prev = *temp;
                        temp++;
                        continue;
                    }
                    continue;
                } else if (in_frontmatter_list) {
                    // end of list

                    fputs(":\n", fout);
                    prev = '\n';
                    in_frontmatter_list = !in_frontmatter_list;
                }

                if (prev == '\n') {
                    fputc(':', fout);
                }
                // print the rest of the line
                while (*temp != '\n') {

                    // print the char
                    fputc(*temp, fout);
                    prev = *temp;
                    temp++;
                }
                // put new line

                fputc('\n', fout);
                prev = '\n';
            }

            // end of properties
            fputs(":END:\n", fout);
        } else {
            // TODO: test this
            // put next and next_next back
            ungetc(next_next_char, fin);
            ungetc(next_char, fin);
            ungetc(curr_char, fin);
        }
    } else {
        // TODO: test this case
        // put curr_char back and move on
        ungetc(curr_char, fin);
    }

    while ((curr_char = fgetc(fin)) != EOF) {

        // TODO: check if temp is a blank line or eof,
        // TODO: if else on major syntax checkers
        // TODO: if in block, don't do any of this
        // TODO: if in blockquote, no codeblock, table?
        // TODO:
        if (curr_char == '\n') {
            // Empty line
            prev = '\n';
            fputc(curr_char, fout);
            continue;
        } else if ((curr_char == '#') && (!in_code_block) &&
                   (prev == '\n' || prev == '\0')) {
            // Header
            // TODO: add a counter to keep track of header level and when to
            // insert TOC
            ungetc(curr_char, fin);
            fgets(curr_line, 1024, fin);

            // Header level and count detection 6->1
            if (strstr(curr_line, "#######") != NULL) {
                // h6
                h6_counter++;
                prev_header = curr_header;
                curr_header = 6;
                //
            } else if (strstr(curr_line, "######") != NULL) {
                // h5
                h5_counter++;
                prev_header = curr_header;
                curr_header = 5;
            } else if (strstr(curr_line, "#####") != NULL) {
                // h4
                h4_counter++;
                prev_header = curr_header;
                curr_header = 4;
            } else if (strstr(curr_line, "###") != NULL) {
                // h3
                h3_counter++;
                prev_header = curr_header;
                curr_header = 3;
            } else if (strstr(curr_line, "##") != NULL) {
                // h2
                h2_counter++;
                prev_header = curr_header;
                curr_header = 2;
            } else if (strstr(curr_line, "#") != NULL) {
                // h1
                h1_counter++;
                prev_header = curr_header;
                curr_header = 1;
            }

            if (h1_counter == 1 && prev_header == 1 && curr_header > 1) {
                // we are at the first h2
                // insert TOC here
                // we want to insert this after first h1 , before first ** or
                // before EOF somehow keep track of the header levels.
                // TODO: insert a table of contents section
                fputs("\n** Table of Contents :TOC:\n\n", fout);
            }

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
        // ``` language
        // ```
        // into
        // #+BEGIN_SRC language
        // #+END_SRC
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

            // if buffer contains ''', we have a codeblock, if in_code_block
            // is false, we are in first line, if true, we want second.

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
                    // ! Is there a better way to do this? no need for curr
                    // pos or new str, strcpy directly into new line?

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
        if (!in_code || !in_code_block) {
            // check for emphasis
            // bold
            // Italics
            // strikethroughs
            // make this only run if it is paragraph or within a quote?

            //  if * or _ we need to check the next too
            if (curr_char == '*' || curr_char == '_') {
                // ensure next is the same
                // also check if prev is space
                // this could mess up some random * char that are not for
                // bold
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
                    // we have "consumed" both bold chars so we can simply
                    // move on

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
                    // we have "consumed" both bold chars so we can simply
                    // move on
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
            } else if (curr_char == '|') {
                // TODO: tables
                int next_char = fgetc(fin);
                int next_next_char = fgetc(fin);

                if ((next_char == ' ' && next_next_char == '-') ||
                    (next_char == '-')) {
                    // we are in a table separator
                    // we want to replce all | except bookends with +
                    // put chars back
                    ungetc(next_next_char, fin);
                    ungetc(next_char, fin);
                    ungetc(curr_char, fin);

                    // get whole line
                    fgets(curr_line, 1024, fin);
                    //
                    // naming here is terrible, curr_pos is of new str, remp
                    // is of curr_line
                    char *curr_pos = new_str;
                    // here we want to iterate through curr_line, when we
                    // get to a space, skip
                    temp = curr_line;
                    // copy first |
                    *curr_pos = *temp;
                    curr_pos++;
                    temp++;
                    while (*temp != '\n' || '\0') {
                        if (*temp == '|' && (*(temp + 1) != '\n' || '\0')) {
                            *curr_pos = '+';
                            curr_pos++;
                            temp++;
                            continue;
                        } else if (*temp == ' ') {
                            // want to skip spaces
                            // TODO: make spaces "delete"
                            temp++;
                            continue;
                        }
                        *curr_pos = *temp;
                        temp++;
                        curr_pos++;
                    }

                    *curr_pos = *temp;
                    fputs(new_str, fout);
                    prev = '\n';
                    continue;

                    // print the line
                } else {
                    // put back next chars
                    //
                    ungetc(next_next_char, fin);
                    ungetc(next_char, fin);
                }
            } else if (curr_char == '>' && (prev == '\n' || prev == '\0')) {
                // blockquote
                // want a global in_quote - no tables, codeblock
                // TODO: confirm prev cannot be a space or tab
                if (!in_quote) {
                    // we are at first >
                    // set to #+BEGIN_QUOTE\n;
                    char new_line[] = "#+BEGIN_QUOTE\n";
                    in_quote = !in_quote;
                    fputs(new_line, fout);
                    prev = curr_char;
                    continue;
                } else {
                    // skip this >
                    prev = curr_char;
                    continue;
                }
            } else if (in_quote && (curr_char != '>') &&
                       (prev == '\n' || prev == '\0')) {
                // TODO: print #+END_QUOTE \n
                //  set prev to \n

                char new_line[] = "#+END_QUOTE\n";
                in_quote = !in_quote;
                fputs(new_line, fout);
                prev = '\n';
            }

        } // if (!in_code || !in_code_block)
        // TODO add blockquote ending
        // unordered and ordered lists are the same
        // checkboxes are the same

        fputc(curr_char, fout);
        prev = curr_char;
        // print curr char, set prev to curr
    } // while
    // TODO: links - image, url
    // TODO: parameters
    // TODO: highlight - not in org mode, so bold?

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
