/**
 * @file parse_args.c
 * @brief Parses command line arguments.
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#include "parse_args.h"
#include "config.h"
#include "execute_conversion.h"
#include "print_usage.h"
#include <getopt.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Parses command line arguments.
 * @param{int} argc Number of command line arguments.
 * @param{char*} argv Array of command line arguments.
 * @param {Config*} config Configuration object to parse into.
 * @return {int} Staus of parse: 0:complete, 1: error, -2: --help flag
 * */
int parse_args(int argc, char *argv[], Config *config) {
    memset(config, 0, sizeof(Config));

    // long options
    static struct option long_options[] = {
        {"output", required_argument, 0, 'o'},
        {"recursive", no_argument, 0, 'r'},
        {"force", no_argument, 0, 'f'},
        {"toc", no_argument, 0, 't'},
        {"prop", no_argument, 0, 'p'},
        {"dry-run", no_argument, 0, 'd'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}};

    int opt;
    optind = 1; // reset for testing

    // parse options:
    while ((opt = getopt_long(argc, argv, "o:rftpdh", long_options, NULL)) !=
           -1) {
        switch (opt) {
        case 'o':
            config->target_path = optarg;
            break;
        case 'r':
            config->recursive = 1;
            break;
        case 'f':
            config->force = 1;
            break;
        case 't':
            config->toc = 1;
            break;
        case 'p':
            config->prop = 1;
            break;
        case 'd':
            config->dry_run = 1;
            break;
        case 'h':
            print_usage(argv[0]);
            return -2;
        default:
            fprintf(stderr, "Error: Invalid arguments\n\n");
            print_usage(argv[0]);
            return 1;
        } // switch
    } // while

    int remaining_args = argc - optind;

    if (remaining_args < 1) {
        fprintf(stderr,
                "Error: No source or target file or directory specified\n\n");
        print_usage(argv[0]);
        return 1;
    } // if

    config->source_path = argv[optind];

    if (remaining_args >= 2) {
        config->target_path = argv[optind + 1];
    } // if

    return execute_conversion(config);

} // parse_args()
