/**
 * @file file_test_utils.h
 * @brief Helper functions for file testing.
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#ifndef FILE_TEST_UTILS_H_
#define FILE_TEST_UTILS_H_

/**
 * @brief Helper funtion to read a file into string.
 *
 * @param {char*} filepath File to open.
 *
 * @return{char*} String of file contents.
 *         NULL on memory allocation failure.
 *         NULL on file opening failure.
 * */
char *read_file(const char *filepath);

/**
 * @brief Helper to compare two files.
 *
 * @param {char*} expected_path Path to file of expected output.
 * @param {char*} actual_path Path to file of actual output.
 *
 * @see read_file()
 * */
void assert_files_equal(const char *expected_path, const char *actual_path);

#endif // FILE_TEST_UTILS_H_
