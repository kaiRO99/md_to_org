/**
 * @file rm_rf.h
 * @brief Recursively deletes directory (rm -rf)
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#ifndef RM_RF_H_
#define RM_RF_H_
#define _XOPEN_SOURCE 500
#include <ftw.h>
#include <sys/stat.h>

int rm_rf(const char *path);
int delete_dir(const char *dir);

#endif // RM_RF_H_
