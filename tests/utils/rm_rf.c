/**
 * @file rm_rf.c
 * @brief Recursively deletes directory (rm -rf)
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#include "rm_rf.h"
#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <stdio.h>
#include <sys/stat.h>

static int rm_entry(const char *path, const struct stat *sb, int typeflag,
                    struct FTW *ftwbuf) {
    (void)sb;
    (void)typeflag;
    (void)ftwbuf;
    return remove(path);
} // rm_entry()

int rm_rf(const char *path) {
    // FTW_DEPTH: visit files before dir
    // FTW_PHYS: avoid symlinks
    return nftw(path, rm_entry, 64, FTW_DEPTH | FTW_PHYS);
} // rm_rf()

int delete_dir(const char *dir) {
    if (rm_rf(dir) != 0) {
        perror("rm_rf failed");
        return 1;
    }
    return 0;
} // delete_dir()
