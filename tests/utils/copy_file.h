/**
 * @file copy_file.h
 * @brief Copies a file from one path to another.
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#ifndef COPY_FILE_H_
#define COPY_FILE_H_

/**
 * @brief Copies a file.
 * @param {char*} src File to copy.
 * @param {cjar*} dst Destinatioon to copy to.
 * @note wb, rb, for binary mode (windows)
 * @note could use sendfile() (<sys/sendfile.h>) on linux
 * */
int copy_file(const char *src, const char *dst);

#endif // COPY_FILE_H_
