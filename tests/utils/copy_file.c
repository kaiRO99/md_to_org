/**
 * @file copy_file.c
 * @brief Copies a file from one path to another.
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#include "copy_file.h"
#include <stdio.h>

/**
 * @brief Copies a file.
 * @param {char*} src File to copy.
 * @param {cjar*} dst Destinatioon to copy to.
 * @note wb, rb, for binary mode (windows)
 * @note could use sendfile() (<sys/sendfile.h>) on linux
 * */
int copy_file(const char *src, const char *dst) {
    FILE *in = fopen(src, "rb");
    FILE *out = fopen(dst, "wb");

    char buffer[4096];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), in)) > 0) {
        fwrite(buffer, 1, n, out);
    } // while
    fclose(in);
    fclose(out);
    return 0;
} // copy_file()
