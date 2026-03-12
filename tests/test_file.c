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
#define _POSIX_C_SOURCE 200809L
#define _XOPEN_SOURCE 500
#include "config.h"
#include "file_test_utils.h"
#include "parse_file.h"
#include "rm_rf.h"
#include "unity.h"
#include "unity_internals.h"
#include <ftw.h>
#include <string.h>
#include <sys/stat.h>

// Configuration to pass to parse_file()
Config config = {
    .source_path = NULL,
    .target_path = NULL,
    .dry_run = 0,
    .recursive = 0,
    .toc = 0,
    .prop = 0,
    .force = 0,
};

/**
 * @brief Setup before each test
 * */
void setUp(void) {
    // Clean up any previous test output files
    /* remove("tests/fixtures/output_temp.org"); */
    // reset config
    config.source_path = NULL;
    config.target_path = NULL;
    config.dry_run = 0;
    config.recursive = 0;
    config.toc = 0;
    config.prop = 0;
    config.force = 0;
    /* delete_dir("tests/fixtures/outputs"); */
    mkdir("tests/fixtures/outputs", 0755);
} // setUp()

/**
 * @brief Cleanup after each test
 * */
void tearDown(void) {
    // Clean up test output files
    /* remove("tests/fixtures/output_temp.org"); */
    // reset config
    config.source_path = NULL;
    config.target_path = NULL;
    config.dry_run = 0;
    config.recursive = 0;
    config.toc = 0;
    config.prop = 0;
    config.force = 0;
    delete_dir("tests/fixtures/outputs");
} // tearDown()

/**
 * @brief Test for header conversions
 * @note Tests:
 *       - w/ table of contents
 *       - w/o table of contents

 * */
void test_headers(void) {
    const char *input = "tests/fixtures/inputs/headers.md";
    config.source_path = strdup(input);

    // w/ table of contents
    const char *output = "tests/fixtures/outputs/output_temp.org";
    const char *expected = "tests/fixtures/expected/headers.org";
    config.toc = 0;
    config.target_path = strdup(output);
    int result = parse_file(input, output, &config);
    TEST_ASSERT_EQUAL(0, result);
    assert_files_equal(expected, output);

    // w/o table of contents
    const char *expected_toc = "tests/fixtures/expected/headers_toc.org";
    const char *output_toc = "tests/fixtures/outputs/output_temp_toc.org";
    config.target_path = strdup(output_toc);
    config.toc = 1;
    int result_toc = parse_file(input, output_toc, &config);
    TEST_ASSERT_EQUAL(0, result_toc);
    assert_files_equal(expected_toc, output_toc);

} // test_headers()

/**
 * @brief Test for code and quote block conversions
 *
 * @see parse_file()
 * */
void test_blocks(void) {
    const char *input = "tests/fixtures/inputs/blocks.md";
    const char *output = "tests/fixtures/outputs/output_temp.org";
    const char *expected = "tests/fixtures/expected/blocks.org";
    config.source_path = strdup(input);
    config.target_path = strdup(output);

    int result = parse_file(input, output, &config);
    TEST_ASSERT_EQUAL(0, result);
    assert_files_equal(expected, output);
} // test_blocks()

/**
 * @brief Test for property conversions
 * @note Tests:
 *       - w/o properties
 *       - w/ properties
 * @see parse_file()
 * */
void test_props(void) {
    const char *input = "tests/fixtures/inputs/properties.md";
    config.source_path = strdup(input);

    // w/o properties
    const char *output = "tests/fixtures/outputs/output_temp.org";
    const char *expected = "tests/fixtures/expected/properties.org";
    config.target_path = strdup(output);
    config.prop = 0;
    int result = parse_file(input, output, &config);
    TEST_ASSERT_EQUAL(0, result);
    assert_files_equal(expected, output);

    // w/ properties
    const char *output_prop = "tests/fixtures/outputs/output_temp_prop.org";
    const char *expected_prop = "tests/fixtures/expected/properties_prop.org";
    config.target_path = strdup(output_prop);
    config.prop = 1;
    int result_prop = parse_file(input, output_prop, &config);
    TEST_ASSERT_EQUAL(0, result_prop);
    assert_files_equal(expected_prop, output_prop);
} // test_lists()

/**
 * @brief Test for table conversions
 *
 * @see parse_file()
 * */
void test_tables(void) {
    const char *input = "tests/fixtures/inputs/tables.md";
    const char *output = "tests/fixtures/outputs/output_temp.org";
    const char *expected = "tests/fixtures/expected/tables.org";
    config.source_path = strdup(input);
    config.target_path = strdup(output);

    int result = parse_file(input, output, &config);
    TEST_ASSERT_EQUAL(0, result);
    assert_files_equal(expected, output);
} // test_tables()

/**
 * @brief Test for full file.
 * @note  Tests:
 *        - w/o toc, w/o properties
 *        - w/toc, w/o properties
 *        - w/toc, w/ properties
 * @see parse_file()
 * */
void test_all(void) {
    const char *input = "tests/fixtures/inputs/all.md";
    config.source_path = strdup(input);

    // w/o toc, w/o properties
    const char *output = "tests/fixtures/outputs/output_temp.org";
    const char *expected = "tests/fixtures/expected/all.org";
    config.target_path = strdup(output);
    config.toc = 0;
    config.prop = 0;
    int result = parse_file(input, output, &config);
    TEST_ASSERT_EQUAL(0, result);
    assert_files_equal(expected, output);

    // w/ table of contents, w/o properties
    const char *output_toc = "tests/fixtures/outputs/output_temp_toc.org";
    const char *expected_toc = "tests/fixtures/expected/all_toc.org";
    config.target_path = strdup(output_toc);
    config.toc = 1;
    config.prop = 0;
    int result_toc = parse_file(input, output_toc, &config);
    TEST_ASSERT_EQUAL(0, result_toc);
    assert_files_equal(expected_toc, output_toc);

    // w/ table of contents, w/ properties
    const char *output_toc_prop =
        "tests/fixtures/outputs/output_temp_toc_prop.org";
    const char *expected_toc_prop = "tests/fixtures/expected/all_toc_prop.org";
    config.target_path = strdup(output_toc_prop);
    config.toc = 1;
    config.prop = 1;
    int result_toc_prop = parse_file(input, output_toc_prop, &config);
    TEST_ASSERT_EQUAL(0, result_toc_prop);
    assert_files_equal(expected_toc_prop, output_toc_prop);
} // test_all()

/**
 * @brief Test for error handling
 *
 * @see parse_file()
 * */
void test_nonexistent_file(void) {
    int result = parse_file("nonexistent.md", "output.org", &config);
    TEST_ASSERT_NOT_EQUAL(0, result);
    // Should fail
} // test_nonexistent_file();

/**
 * @brief Test for non-.md file
 * @see parse_file()
 * */
void test_non_md(void) {
    int result = parse_file("nonexistent.txt",
                            "tests/fixtures/outputs/output.org", &config);
    TEST_ASSERT_NOT_EQUAL(0, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_headers);
    RUN_TEST(test_blocks);
    RUN_TEST(test_tables);
    RUN_TEST(test_props);
    RUN_TEST(test_all);
    RUN_TEST(test_nonexistent_file);
    RUN_TEST(test_non_md);
    return UNITY_END();
} // main()
