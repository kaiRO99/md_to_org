/**
 * @file main.c
 * @brief Entry point for md_to_org
 * @author Kai Ryall Ota
 * @date Feb 2026
 * */
#include "config.h"
#include "parse_args.h"

/**
 * @brief Entry point for md_to_org
 * @param{int} argc number of arguments passed.
 * @param{char*} argv argv[1]: markdown file, argv[2]: target file
 * */
int main(int argc, char *argv[]) {
    Config config;

    return parse_args(argc, argv, &config);
} // main()
