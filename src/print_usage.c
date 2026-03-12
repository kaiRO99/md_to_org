/**
 * @file print_usage.c
 * @brief Prints cli options
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#include "print_usage.h"
#include <stdio.h>

/**
 * @brief Prints options for cli
 * @note Triggered by -h flag
 * @param{*char} prog_name Name of program
 * */
void print_usage(const char *prog_name) {
    printf("Usage: %s [OPTIONS] [TARGET] SOURCE\n\n", prog_name);
    printf("Convert Markdown files to Org-mode format.\n\n");
    printf("Arguments:\n");
    printf("  SOURCE             File or directory to convert\n");
    printf("  TARGET             Output file or directory (optional)\n\n");
    printf("Options:\n");
    printf("  -o, --output PATH  Output directory for batch conversion\n");
    printf("  -p, --prop         Includes the properties section, if it "
           "exists\n");
    printf("  -t, --toc          Includes a table of contents (:toc:) after "
           "the first level 1 header\n");
    printf("  -r, --recursive    Process directories recursively\n");
    printf("  -f, --force        Overwrite existing files\n");
    /* printf("  -v, --verbose      Verbose output\n"); */
    printf("  -d, --dry-run      Show what would be done without doing it\n");
    printf("  -h, --help         Show this help message\n\n");
    printf("Examples:\n");
    printf("  %s input.md                    Convert single file (output: "
           "input.org)\n",
           prog_name);
    printf("  %s -o output.org input.md      Convert with specific output "
           "name\n",
           prog_name);
    printf(
        "  %s docs/                       Convert all .md files in directory\n",
        prog_name);
    printf(
        "  %s -o out/ docs/               Convert directory docs/ to out/ \n",
        prog_name);
    printf("  %s -r docs/                    Convert directory recursively\n",
           prog_name);
} // print_usage()
