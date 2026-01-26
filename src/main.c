#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_file(const char *filename);

bool in_block = false;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }
    parse_file(argv[1]);

    exit(0);
}

/**
 * @brief
 *
 * @param
 * @return
 * */
void parse_file(const char *filename) {
    // file pointer
    FILE *fin = fopen(filename, "r");
    FILE *fout = fopen("src/test2.md", "w");
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
    while (fgets(curr_line, 1024, fin) != NULL) {
        temp = curr_line;
        // Headers
        while (*temp == '#') {
            *temp = '*';
            (temp)++;
        }
        // NOTE: Codeblock
        // ``` language
        // ```
        // into
        // #+BEGIN_SRC language
        // #+END_SRC
        if (*temp == '`' && *(temp + 1) == '`' && *(temp + 2) == '`') {
            // entering codeblock, need to keep language.
            if (in_block == false) {
                size_t original_len = strlen(curr_line);
                size_t new_len = original_len + 9;

                char *curr_pos = new_str;
                // copy #+BEGIN_SRC
                strncpy(curr_pos, "#+BEGIN_SRC ", 12);
                curr_pos += 12;

                // ! Is there a better way to do this? no need for curr pos or
                // new str, strcpy directly into new line?

                // copy rest of string
                strcpy(curr_pos, curr_line + 3);
                // set curr_line to new str
                strcpy(curr_line, new_str);
                in_block = true;

            } else {
                // we are at end of block
                // need to also print the \n
                char new_line[] = "#+END_SRC\n";
                strcpy(curr_line, new_line);
                in_block = false;
            }
        }

        // TODO: parameters
        // TODO: bold
        // need to parse entire line
        // TODO: Italics
        // TODO: strikethroughs
        // TODO: blockquote
        // TODO: highlight
        // TODO: Lists
        // TODO: checkboxes
        // TODO:
        fputs(curr_line, fout);
        memset(new_str, 0, strlen(new_str));
        memset(curr_line, 0, strlen(curr_line));
    }

    // char *temp;
    // bool in_block = false; // to check if currently in a block
    // int block_type = 0;    // 0: null, 1:code, 2:
    //
    //
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
 * @brief Writes a line to the output file.
 *
 * @param */
// void write_line() {}
