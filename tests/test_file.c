/**
 * @file test_file.c
 * @brief Tests for full file conversion from markdown to Org-mode.
 * @author Kai Ryall Ota
 * @date Feb 2026
 * @note Tests:
 *       - Tables
 *       - Code, quote blocks
 *       - Headers
 *       - Properties
 *       - Combined
 * */
#include "../include/parse_file.h"
#include "unity.h"
#include "unity_internals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Setup before each test
 * */
void setUp(void) {
    // Clean up any previous test output files
    remove("tests/fixtures/output_temp.org");
} // setUp()

/**
 * @brief Cleanup after each test
 * */
void tearDown(void) {
    // Clean up test output files
    remove("tests/fixtures/output_temp.org");
} // tearDown()

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

/**
 * @brief Test for header conversions
 *
 * @see parse_file()
 * */
void test_headers(void) {
    const char *input = "tests/fixtures/inputs/headers.md";
    const char *output = "tests/fixtures/output_temp.org";
    const char *expected = "tests/fixtures/expected/headers.org";
    int result = parse_file(input, output);
    TEST_ASSERT_EQUAL(0, result);
    assert_files_equal(expected, output);
} // test_headers()

/**
 * @brief Test for code and quote block conversions
 *
 * @see parse_file()
 * */
void test_blocks(void) {
    const char *input = "tests/fixtures/inputs/blocks.md";
    const char *output = "tests/fixtures/output_temp.org";
    const char *expected = "tests/fixtures/expected/blocks.org";
    int result = parse_file(input, output);
    TEST_ASSERT_EQUAL(0, result);
    assert_files_equal(expected, output);
} // test_blocks()

/**
 * @brief Test for property conversions
 *
 * @see parse_file()
 * */
void test_props(void) {
    const char *input = "tests/fixtures/inputs/properties.md";
    const char *output = "tests/fixtures/output_temp.org";
    const char *expected = "tests/fixtures/expected/properties.org";
    int result = parse_file(input, output);
    TEST_ASSERT_EQUAL(0, result);
    assert_files_equal(expected, output);
} // test_lists()

/**
 * @brief Test for table conversions
 *
 * @see parse_file()
 * */
void test_tables(void) {
    const char *input = "tests/fixtures/inputs/tables.md";
    const char *output = "tests/fixtures/output_temp.org";
    const char *expected = "tests/fixtures/expected/tables.org";
    int result = parse_file(input, output);
    TEST_ASSERT_EQUAL(0, result);
    assert_files_equal(expected, output);
} // test_tables()

/**
 * @brief Test for full file.
 *
 * @see parse_file()
 * */
void test_all(void) {
    const char *input = "tests/fixtures/inputs/all.md";
    const char *output = "tests/fixtures/output_temp.org";
    const char *expected = "tests/fixtures/expected/all.org";
    int result = parse_file(input, output);
    TEST_ASSERT_EQUAL(0, result);
    assert_files_equal(expected, output);
} // test_all()

/**
 * @brief Test for error handling
 *
 * @see parse_file()
 * */
void test_nonexistent_file(void) {
    int result = parse_file("nonexistent.md", "output.org");
    TEST_ASSERT_NOT_EQUAL(0, result);
    // Should fail
} // test_nonexistent_file();

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_headers);
    RUN_TEST(test_blocks);
    RUN_TEST(test_tables);
    RUN_TEST(test_props);
    RUN_TEST(test_all);
    RUN_TEST(test_nonexistent_file);
    return UNITY_END();
} // main()
