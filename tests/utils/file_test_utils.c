/**
 * @file file_test_utils.c
 * @brief Helper functions for file tests
 * @author Kai Ryall Ota
 * @date March 2026
 * */
#include "file_test_utils.h"
#include "unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Helper funtion to read a file into string.
 *
 * @param {char*} filepath File to open.
 *
 * @return{char*} String of file contents.
 *         NULL on memory allocation failure.
 *         NULL on file opening failure.
 * */
char *read_file(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        return NULL;
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    // Allocate buffer and read
    char *content = malloc(size + 1);
    if (!content) {
        fclose(file);
        return NULL;
    }
    fread(content, 1, size, file);
    content[size] = '\0';
    fclose(file);
    return content;
} // read_file()

/**
 * @brief Helper to compare two files.
 *
 * @param {char*} expected_path Path to file of expected output.
 * @param {char*} actual_path Path to file of actual output.
 *
 * @see read_file()
 * */
void assert_files_equal(const char *expected_path, const char *actual_path) {
    char *expected = read_file(expected_path);
    char *actual = read_file(actual_path);
    TEST_ASSERT_NOT_NULL_MESSAGE(expected, "Could not read expected file");
    TEST_ASSERT_NOT_NULL_MESSAGE(actual, "Could not read actual output file");
    // Compare content
    TEST_ASSERT_EQUAL_STRING_MESSAGE(expected, actual,
                                     "File contents do not match");
    free(expected);
    free(actual);
} // Assert_files_equal
