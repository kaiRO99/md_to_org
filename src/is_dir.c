/**
 * @file is_dir.c
 * @brief Checks if a path is a directory.
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#include "is_dir.h"
#include <sys/stat.h>
/**
 * @brief Checks if path is a directory
 * @param{char*} path Path to check
 * @return{int} 1 if it a dir, 0 if not
 * */
int is_dir(const char *path) {
    struct stat sb;
    if (stat(path, &sb) != 0) {
        return 0;
    } // if
    return S_ISDIR(sb.st_mode);
} // is_dir()
