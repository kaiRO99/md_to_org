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
bool in_link_alt = false;
bool in_link_path = false;
bool in_link = false;
int h6_counter = 0;
int h5_counter = 0;
int h4_counter = 0;
int h3_counter = 0;
int h2_counter = 0;
int h1_counter = 0;
int curr_header = 0;
int prev_header = 0;

/**
 * @brief Parses a mardown file and converts it to org mode.
 *
 * @param {*char} filename Filename of markdown file to convert.
 * @param {*char} target_filename Filename of org mode to output to.
 *
 * BUG: local links may break
 * */
void parse_file(const char *filename, const char *target_filename) {

    FILE *fin = fopen(filename, "r");         // Input file pointer
    FILE *fout = fopen(target_filename, "w"); // Output file pointer

    // File pointer validation
    if (fin == NULL) {
        printf("Error: Could not open file.\n");
        exit(EXIT_FAILURE);
    } // if
    if (fout == NULL) {
        printf("Error: Could not open file.\n");
        exit(EXIT_FAILURE);
    } // if

    // Malloc pointers for parsing files
    char *curr_line = (char *)malloc(sizeof(char) * 1024); // Current line
    char *new_str = (char *)malloc(sizeof(char) * 1024);   // Temp buffer
    char *alt_text =
        (char *)malloc(sizeof(char) * 1024); // Temp buffer for link alt text
    char *url =
        (char *)malloc(sizeof(char) * 1024); // Temp buffer for link urls

    // Temporary pointers and variables
    char *temp;      // For traversing line
    int curr_char;   // For current charachter being parsed
    int prev = '\n'; // To keep track of previous character

    // Properties
    curr_char = fgetc(fin);
    if (curr_char == '-') {
        int next_char = fgetc(fin);
        int next_next_char = fgetc(fin);
        if (next_char == '-' && next_next_char == '-') {
            // we have yaml frontmatter
            /**
             * NOTE: Do I want to use :PROPERTIES: and #+keywords?- for now, use
             * :PROPERTIES:
             * NOTE: may want have #+ keywords for title, tags, and
             * related
             * NOTE: if It has a "class", add a tag for school cause
             *
             * BUG: local file links will break
             * BUG: A file that starts with line --- but no frontmatter
             * */
            in_frontmatter = !in_frontmatter;
            fputs(":PROPERTIES:\n", fout);
            fgets(curr_line, 1024, fin); // Discard rest of line

            while (strstr(fgets(curr_line, 1024, fin), "---") == NULL) {
                temp = curr_line;
                // BUG: will this skip whitespace in tags?
                while (*temp == ' ' || *temp == '\t') {
                    prev = *temp;
                    temp++;
                } // while
                // Check for a list
                if (*temp == '-' && !in_frontmatter_list) {
                    // First item in a list
                    in_frontmatter_list = !in_frontmatter_list;
                    fseek(fout, -1,
                          SEEK_CUR); // Append to previous line
                    fputs(" :", fout);
                    prev = ' ';
                    temp = temp + 2; // skip space after -
                    while (*temp != '\n') {
                        fputc(*temp, fout);
                        prev = *temp;
                        temp++;
                        continue;
                    } // while
                    continue;
                } else if (*temp == '-' && in_frontmatter_list == true) {
                    // Not first list item
                    fputs(":", fout);
                    prev = ' ';
                    temp = temp + 2; // skip space after -
                    while (*temp != '\n') {
                        // copy over
                        fputc(*temp, fout);
                        prev = *temp;
                        temp++;
                        continue;
                    } // while
                    continue;
                } else if (in_frontmatter_list) {
                    // End of list
                    fputs(":\n", fout);
                    prev = '\n';
                    in_frontmatter_list = !in_frontmatter_list;
                } // else if

                // Prepend : to start of each proerty name
                if (prev == '\n') {
                    fputc(':', fout);
                } // if

                // Copy text to output file
                while (*temp != '\n') {
                    fputc(*temp, fout);
                    prev = *temp;
                    temp++;
                } // while

                // Add new line char
                fputc('\n', fout);
                prev = '\n';
            } // while

            // Close Properties
            fputs(":END:\n", fout);
        } else {
            // TODO: test this
            // Put back chars if no property section
            ungetc(next_next_char, fin);
            ungetc(next_char, fin);
            ungetc(curr_char, fin);
        } // if else
    } else {
        // TODO: test this case
        // No Property section
        ungetc(curr_char, fin);
    } // if else

    while ((curr_char = fgetc(fin)) != EOF) {

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
            } // else if

            if (h1_counter == 1 && prev_header == 1 && curr_header > 1) {
                // End of first H1
                // Insert TOC
                fputs("\n** Table of Contents :TOC:\n\n", fout);
            } // if

            // Change #->*
            temp = curr_line;
            while (*temp == '#') {
                *temp = '*';
                (temp)++;
            } // while
            fputs(curr_line, fout);
            prev = '\n';
            continue;
        } // else if
        /**
         * Codeblock
         * ``` language -> #+BEGIN_SRC language
         * ``` -> #+END_SRC
         **/
        if ((curr_char == '`') && (prev == '\n' || prev == '\0')) {
            char buffer[4];
            buffer[0] = (char)curr_char;
            for (int i = 1; i < 3; i++) {
                int c = fgetc(fin);
                if (c == EOF) {
                    break;
                } // if
                buffer[i] = (char)c;
            } // for

            /* if buffer contains ''', we have a codeblock, if in_code_block
             * is false, we are in first line, if true, we want second.
             */
            if (strcmp(buffer, "```") == 0) {
                // Put the buffer back
                for (int i = 2; i >= 0; i--) {
                    ungetc(buffer[i], fin);
                }
                // Get the full line and process
                fgets(curr_line, 1024, fin);
                if (in_code_block == false) {

                    char *curr_pos = new_str;
                    // copy #+BEGIN_SRC
                    strncpy(curr_pos, "#+BEGIN_SRC ", 12);
                    curr_pos += 12;
                    // NOTE: Is there a better way to do this? no need for curr
                    // pos or new str, strcpy directly into new line?

                    strcpy(curr_pos, curr_line + 3);
                    strcpy(curr_line, new_str);
                    in_code_block = true;
                    fputs(curr_line, fout);
                    prev = '\n';
                    continue;
                } else {
                    // End of block
                    char new_line[] = "#+END_SRC\n";
                    strcpy(curr_line, new_line);
                    in_code_block = false;
                    fputs(curr_line, fout);
                    prev = '\n';
                    continue;
                } // if else
            } // if
        } else if (curr_char == '`') {
            // Normal code ` -> ~, other emphasis does not apply
            in_code = !in_code;
            fputc('~', fout);
            prev = '~';
            continue;
        } // else if

        if (!in_code || !in_code_block) {
            // Bold, Italics, Strikethrough, Links, Table, Blockquote

            if (in_link && in_link_alt) {
                // In a Link alt text
                if (curr_char == ']') {
                    // End of Alt text
                    in_link_alt = !in_link_alt;
                    prev = curr_char;
                    continue;
                } // if
                // Copy alt text to buffer
                *temp = curr_char;
                temp++;
                prev = curr_char;
                continue;

            } else if (in_link && in_link_path) {
                // in Link URL
                if (curr_char == ')' && in_link) {
                    in_link_path = !in_link_path;
                    in_link = !in_link;
                    // print the alt and url
                    fprintf(fout, "[[%s][%s]]", url, alt_text);
                    prev = curr_char;
                    memset(url, 0, strlen(url));
                    memset(alt_text, 0, strlen(alt_text));
                    continue;
                } // if
                // Copy char to url
                *temp = curr_char;
                temp++;
                prev = curr_char;
                continue;
            } // else if
            //  if * or _
            if (curr_char == '*' || curr_char == '_') {
                // ensure next is the same
                // also check if prev is space
                // NOTE: could mess up some random * that are not for bold
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
                    } // else if
                    prev = next_char;
                    continue;
                } else {
                    // Italics
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
                    } // if else
                    prev = curr_char;
                    continue;
                } // if else
            } else if (curr_char == '~') {
                // Strikethrough
                int next_char = fgetc(fin);
                if (next_char == curr_char) {
                    if ((prev != ' ' || prev != '\n' || prev != '\t' ||
                         prev != '\0' || prev != '\f') &&
                        !in_sthrough) {
                        fputc(' ', fout);
                        fputc('+', fout);
                        in_sthrough = !in_sthrough;
                    } else if (in_sthrough) {
                        // we want * followed by a space
                        fputc('+', fout);
                        fputc(' ', fout);
                        in_sthrough = !in_sthrough;
                    } // else if
                    prev = next_char;
                    continue;
                } else {
                    // Italics
                    ungetc(next_char, fin);
                    fputc(curr_char, fout);
                    prev = curr_char;
                    continue; // go to next char
                } // if else
            } else if (curr_char == '|') {
                // Tables
                int next_char = fgetc(fin);
                int next_next_char = fgetc(fin);

                if ((next_char == ' ' && next_next_char == '-') ||
                    (next_char == '-')) {
                    // Replace all | with +, except bookends
                    ungetc(next_next_char, fin);
                    ungetc(next_char, fin);
                    ungetc(curr_char, fin);
                    fgets(curr_line, 1024, fin);
                    char *curr_pos = new_str;
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
                            temp++;
                            continue;
                        } // else if
                        *curr_pos = *temp;
                        temp++;
                        curr_pos++;
                    } // while
                    *curr_pos = *temp;
                    fputs(new_str, fout);
                    prev = '\n';
                    continue;
                } else {
                    ungetc(next_next_char, fin);
                    ungetc(next_char, fin);
                } // if else
            } else if (curr_char == '>' && (prev == '\n' || prev == '\0')) {
                // Blockquote
                // TODO: confirm prev cannot be a space or tab
                if (!in_quote) {
                    // First >
                    char new_line[] = "#+BEGIN_QUOTE\n";
                    in_quote = !in_quote;
                    fputs(new_line, fout);
                    prev = curr_char;
                    continue;
                } else {
                    // Skip the >
                    prev = curr_char;
                    continue;
                } // if else
            } else if (in_quote && (curr_char != '>') &&
                       (prev == '\n' || prev == '\0')) {
                char new_line[] = "#+END_QUOTE\n";
                in_quote = !in_quote;
                fputs(new_line, fout);
                prev = '\n';
            } else if (curr_char == '[') {
                // org mode: [[link][descrptiom]] [[path to local image]]
                // md: [description](link) ![alt text](image path)
                // look for .jpeg, .jpg ,.png, .gif, .webp, .svg,..tif,.tiff ?
                // TODO: test images
                if (prev == '!') {
                    // Image link
                    fseek(fout, -1, SEEK_CUR);
                } // if
                // File/image link alt text
                in_link_alt = !in_link_alt;
                in_link = !in_link;
                prev = curr_char;
                temp = alt_text;
                continue;
            } else if (curr_char == '(' && in_link && !in_link_alt) {
                // we have a url
                in_link_path = !in_link_path;
                prev = curr_char;
                temp = url;
                continue;
            } // if else
        } // if (!in_code || !in_code_block)

        fputc(curr_char, fout);
        prev = curr_char;
    } // while curr_char !=EOF

    if (curr_line) {
        free(curr_line);
    } // if curr_line
    if (new_str) {
        free(new_str);
    } // if new_str
    if (url) {
        free(url);
    } // if url
    if (alt_text) {
        free(alt_text);
    } // if alt_text
    fclose(fout);
    fclose(fin);
} // parse_file

/**
 * @brief Shifts everything after str back by 1
 * */
void delete_char(char *str) { memmove(str, str + 1, strlen(str)); }
