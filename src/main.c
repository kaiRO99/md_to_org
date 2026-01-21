#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parse_file(const char *filename);

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
    char *curr_line = (char *)malloc(sizeof(char) * 1024); // TODO: verify size
    char *temp;
    while (fgets(curr_line, 1024, fin) != NULL) {
        temp = curr_line;
        // TODO: Headers
        while (*temp == '#') {
            *temp = '*';
            (temp)++;
        }

        // TODO: parameters
        // TODO: bold
        // TODO: Italics
        // TODO: strikethroughs
        // TODO: blockquote
        // TODO: highlight
        // TODO: Lists
        // TODO: checkboxes
        // TODO:
        printf("%s\n", curr_line);
        fputs(curr_line, fout);

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
    fclose(fout);
    fclose(fin);
}

/**
 * @brief Writes a line to the output file.
 *
 * @param */
// void write_line() {}
