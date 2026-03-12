/**
 * @file test_cli.c
 * @brief Tests for command line arguments.
 * @author Kai Ryall Ota
 * @date March 2026
 * */
/* #include "test_cli.h" */
#include "config.h"
#include "copy_file.h"
#include "file_test_utils.h"
#include "parse_args.h"
#include "rm_rf.h"
#include "unity.h"
#include "unity_internals.h"
#include <stdio.h>
#include <sys/stat.h>

static int forced = 0; // flag if
// delete output_temp.org, rm_rf?
// delete/replace fixtures/inputs/cli_tests/force/all.org with
// cli_tests/original/all.org delete fixtures/inputs/cli_tests/all.org
// delete "tests/fixtures/inputs/cli_tests/toc/headers.org"
/**
 * @brief Setup before each test
 * */
void setUp(void) {
    mkdir("tests/fixtures/outputs", 0755);
    forced = 0;
} // setUp()

/**
 * @brief Teardown after each test
 * */
void tearDown(void) {
    delete_dir("tests/fixtures/outputs");
    // test_no_flag_file()
    remove("tests/fixtures/inputs/cli_tests/all.org");
    // test_no_flag_dir()
    remove(
        "tests/fixtures/inputs/cli_tests/recursive/recursive_lvl2/blocks.org");
    remove(
        "tests/fixtures/inputs/cli_tests/recursive/recursive_lvl2/headers.org");
    // test_toc_flag
    remove("tests/fixtures/inputs/cli_tests/toc/headers.org");
    // test_props_flag
    remove("tests/fixtures/inputs/cli_tests/props/properties.org");
    // recursive outputs
    remove("tests/fixtures/inputs/cli_tests/recursive/all.org");
    remove(
        "tests/fixtures/inputs/cli_tests/recursive/recursive_lvl2/blocks.org");
    remove(
        "tests/fixtures/inputs/cli_tests/recursive/recursive_lvl2/headers.org");
    remove("tests/fixtures/inputs/cli_tests/recursive/recursive_lvl2/"
           "recursive_lvl3/properties.org");
    remove("tests/fixtures/inputs/cli_tests/recursive/recursive_lvl2/"
           "recursive_lvl3/tables.org");
    // dir test
    remove("tests/fixtures/inputs/cli_tests/recursive/all.org");

    if (forced) {
        // forced
        remove("tests/fixtures/inputs/cli_tests/forced/all.org");
        copy_file("tests/fixtures/inputs/cli_tests/original/all.org",
                  "tests/fixtures/inputs/cli_tests/forced/all.org");

        forced = 0;
    } // if
} // tearDown()

/**
 * @brief Tests no flags (./md_to_org input.md) file source
 * @see parse_args()
 * */
void test_no_flag_file(void) {
    char *argv[] = {"md_to_org", "tests/fixtures/inputs/cli_tests/all.md"};

    int argc = 2;
    Config config = {0};
    int result = parse_args(argc, argv, &config);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_FALSE(config.dry_run);
    TEST_ASSERT_FALSE(config.force);
    TEST_ASSERT_FALSE(config.prop);
    TEST_ASSERT_FALSE(config.toc);
    TEST_ASSERT_FALSE(config.recursive);
    assert_files_equal("tests/fixtures/expected/all.org",
                       "tests/fixtures/inputs/cli_tests/all.org");
} // test_no_flag_flag()

/**
 * @brief Tests no flags, directory source (./md_to_org input.md)
 * @see parse_args()
 * */
void test_no_flag_dir(void) {
    char *argv[] = {"md_to_org",
                    "tests/fixtures/inputs/cli_tests/recursive/recursive_lvl2"};
    int argc = 2;
    Config config = {0};
    int result = parse_args(argc, argv, &config);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_FALSE(config.dry_run);
    TEST_ASSERT_FALSE(config.force);
    TEST_ASSERT_FALSE(config.prop);
    TEST_ASSERT_FALSE(config.toc);
    TEST_ASSERT_FALSE(config.recursive);
    assert_files_equal(
        "tests/fixtures/expected/blocks.org",
        "tests/fixtures/inputs/cli_tests/recursive/recursive_lvl2/blocks.org");
    assert_files_equal(
        "tests/fixtures/expected/headers.org",
        "tests/fixtures/inputs/cli_tests/recursive/recursive_lvl2/headers.org");
} // test_no_flag_dir()

/**
 * @brief Tests output flag (-o/ --output)
 * @see parse_args()
 * */
void test_output_file_flag(void) {
    char *argv[] = {"md_to_org", "-o", "tests/fixtures/outputs/output_temp.org",
                    "tests/fixtures/inputs/cli_tests/all.md"};

    int argc = 4;
    Config config = {0};
    int result = parse_args(argc, argv, &config);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_FALSE(config.dry_run);
    TEST_ASSERT_FALSE(config.force);
    TEST_ASSERT_FALSE(config.prop);
    TEST_ASSERT_FALSE(config.toc);
    TEST_ASSERT_FALSE(config.recursive);
    assert_files_equal("tests/fixtures/expected/all.org",
                       "tests/fixtures/outputs/output_temp.org");
} // test_output_file_flag()

/**
 * @brief Tests output flag (-o/ --output)
 * @see parse_args()
 * */
void test_output_dir_flag(void) {
    char *argv[] = {"md_to_org", "-o", "tests/fixtures/outputs",
                    "tests/fixtures/inputs/cli_tests/recursive/recursive_lvl2"};

    int argc = 4;
    Config config = {0};
    int result = parse_args(argc, argv, &config);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_FALSE(config.dry_run);
    TEST_ASSERT_FALSE(config.force);
    TEST_ASSERT_FALSE(config.prop);
    TEST_ASSERT_FALSE(config.toc);
    TEST_ASSERT_FALSE(config.recursive);
    printf("here\n");
    assert_files_equal("tests/fixtures/expected/blocks.org",
                       "tests/fixtures/outputs/blocks.org");
} // test_output_dir_flag()

/**
 * @brief Tests output flag with no path
 * @see parse_args()
 * @TODO: Ensure proper error handling
 * */
void test_no_output(void) {
    char *argv[] = {"md_to_org", "-o", "tests/fixtures/all.md"};

    int argc = 3;
    Config config = {0};
    int result = parse_args(argc, argv, &config);
    TEST_ASSERT_NOT_EQUAL(0, result);
    TEST_ASSERT_FALSE(config.dry_run);
    TEST_ASSERT_FALSE(config.force);
    TEST_ASSERT_FALSE(config.prop);
    TEST_ASSERT_FALSE(config.toc);
    TEST_ASSERT_FALSE(config.recursive);
} // test_output_flag()

/**
 * @brief Tests dry run flag (-d/ --dry-run)
 * @see parse_args()
 * @note Not sure how to test what is printed - Check manually for now
 *       files in inputs/ all.md, blocks.md, headers.md, properties.md,
 * tables.md
 * */
void test_dry_flag(void) {
    char *argv[] = {"md_to_org", "-d", "tests/fixtures/inputs/"};
    int argc = 3;
    Config config = {0};
    int result = parse_args(argc, argv, &config);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_TRUE(config.dry_run);
    TEST_ASSERT_FALSE(config.force);
    TEST_ASSERT_FALSE(config.prop);
    TEST_ASSERT_FALSE(config.toc);
    TEST_ASSERT_FALSE(config.recursive);
} // test_dry_flag()

/**
 * @brief Tests dry run flag (-d/--dry-run) with recursive (-r/--recursive)
 * @see parse_args()
 * @note Not sure how to test what is printed - Check manually for now
 *       Files in inputs/ all.md, blocks.md, headers.md, properties.md,
 * tables.md files in cli_tests/ all.md files in original/ all.md, all.org files
 * in force/ all.md, all.org
 * @note Might throw an error without force
 * */
void test_dry_recursive_flag(void) {
    char *argv[] = {"md_to_org", "-d", "-r",
                    "tests/fixtures/inputs/cli_tests/recursive"};
    int argc = 4;
    Config config = {0};
    int result = parse_args(argc, argv, &config);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_TRUE(config.dry_run);
    TEST_ASSERT_TRUE(config.recursive);
    TEST_ASSERT_FALSE(config.force);
    TEST_ASSERT_FALSE(config.prop);
    TEST_ASSERT_FALSE(config.toc);
} // test_dry_flag()

/**
 * @brief Tests toc flag (-t/ --toc)
 * @see parse_args()
 * */
void test_toc_flag(void) {

    char *argv[] = {"md_to_org", "-t",
                    "tests/fixtures/inputs/cli_tests/toc/headers.md"};
    int argc = 3;
    Config config = {0};
    int result = parse_args(argc, argv, &config);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_TRUE(config.toc);
    TEST_ASSERT_FALSE(config.dry_run);
    TEST_ASSERT_FALSE(config.recursive);
    TEST_ASSERT_FALSE(config.force);
    TEST_ASSERT_FALSE(config.prop);
    assert_files_equal("tests/fixtures/expected/headers_toc.org",
                       "tests/fixtures/inputs/cli_tests/toc/headers.org");
} // test_toc_flag()

/**
 * @brief Tests properties flag (-p/ --prop)
 * @see parse_args()
 * */
void test_props_flag(void) {
    char *argv[] = {"md_to_org", "-p",
                    "tests/fixtures/inputs/cli_tests/props/properties.md"};
    int argc = 3;
    Config config = {0};
    int result = parse_args(argc, argv, &config);

    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_TRUE(config.prop);
    TEST_ASSERT_FALSE(config.toc);
    TEST_ASSERT_FALSE(config.dry_run);
    TEST_ASSERT_FALSE(config.recursive);
    TEST_ASSERT_FALSE(config.force);
    assert_files_equal("tests/fixtures/expected/properties_prop.org",
                       "tests/fixtures/inputs/cli_tests/props/properties.org");
} // test_props_flag()

/**
 * @brief Tests force flag (-f/ --force)
 * @see parse_args()
 * */
void test_force_flag(void) {
    char *argv[] = {"md_to_org", "--force",
                    "tests/fixtures/inputs/cli_tests/force/all.md"};

    int argc = 3;
    Config config = {0};
    int result = parse_args(argc, argv, &config);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_TRUE(config.force);
    TEST_ASSERT_FALSE(config.prop);
    TEST_ASSERT_FALSE(config.toc);
    TEST_ASSERT_FALSE(config.dry_run);
    TEST_ASSERT_FALSE(config.recursive);
    assert_files_equal("tests/fixtures/expected/all.org",
                       "tests/fixtures/inputs/cli_tests/force/all.org");
} // test_force_flag()

/**
 * @brief Tests help flag (-h/ --help) with a path
 * @see parse_args()
 * @note Test output visually
 * */
void test_help_flag(void) {
    char *argv[] = {"md_to_org", "-h",
                    "tests/fixtures/inputs/cli_tests/all.md"};

    int argc = 3;
    Config config = {0};
    int result = parse_args(argc, argv, &config);
    TEST_ASSERT_EQUAL(-2, result);
} // test_help_flag()

/**
 * @brief Tests help flag (-h/ --help) w/o path
 * @see parse_args()
 * @note Test output visually
 * */
void test_help_flag_no_path(void) {
    char *argv[] = {"md_to_org", "-h"};

    int argc = 2;
    Config config = {0};
    int result = parse_args(argc, argv, &config);
    TEST_ASSERT_EQUAL(-2, result);
} // test_help_flag_no_path()

/**
 * @brief Tests recursive flag (-r/ --recursive)
 * @see parse_args()
 * */
void test_recursive_flag(void) {
    char *argv[] = {"md_to_org", "-r",
                    "tests/fixtures/inputs/cli_tests/recursive/"};
    int argc = 3;
    Config config = {0};
    int result = parse_args(argc, argv, &config);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_FALSE(config.force);
    TEST_ASSERT_FALSE(config.prop);
    TEST_ASSERT_FALSE(config.toc);
    TEST_ASSERT_FALSE(config.dry_run);
    TEST_ASSERT_TRUE(config.recursive);
    // Check for conversions
    assert_files_equal("tests/fixtures/expected/all.org",
                       "tests/fixtures/inputs/cli_tests/recursive/all.org");
    assert_files_equal(
        "tests/fixtures/expected/blocks.org",
        "tests/fixtures/inputs/cli_tests/recursive/recursive_lvl2/blocks.org");
    assert_files_equal("tests/fixtures/expected/properties.org",
                       "tests/fixtures/inputs/cli_tests/recursive/"
                       "recursive_lvl2/recursive_lvl3/properties.org");
} // test_recursive_flag()

/**
 * @brief Tests recursive flag (-r/ --recursive) w/output (-o/ --output)
 * @see parse_args()
 * */
void test_recursive_output_flag(void) {
    char *argv[] = {"md_to_org", "-r", "-o", "tests/fixtures/outputs",
    "tests/fixtures/inputs/cli_tests/recursive"};
    int argc = 5;
    Config config = {0};
    int result = parse_args(argc, argv, &config);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_FALSE(config.force);
    TEST_ASSERT_FALSE(config.prop);
    TEST_ASSERT_FALSE(config.toc);
    TEST_ASSERT_FALSE(config.dry_run);
    TEST_ASSERT_TRUE(config.recursive);
    // Check for conversions
    assert_files_equal("tests/fixtures/expected/all.org",
                       "tests/fixtures/outputs/all.org");
    assert_files_equal("tests/fixtures/expected/blocks.org",
                       "tests/fixtures/outputs/recursive_lvl2/blocks.org");
    assert_files_equal("tests/fixtures/expected/properties.org",
                       "tests/fixtures/outputs/"
                       "recursive_lvl2/recursive_lvl3/properties.org");
} // test_recursive_output_flag()

/**
 * @brief Tests multiple flags
 * @see parse_args()
 * */
void test_multiple_flags(void) {

    char *argv[] = {"md_to_org", "-t",
    "-o",        "tests/fixtures/outputs/output_temp.org",
    "-p",        "tests/fixtures/inputs/cli_tests/all.md"};

    int argc = 6;
    Config config = {0};
    int result = parse_args(argc, argv, &config);
    TEST_ASSERT_EQUAL(0, result);
    TEST_ASSERT_FALSE(config.force);
    TEST_ASSERT_TRUE(config.prop);
    TEST_ASSERT_TRUE(config.toc);
    TEST_ASSERT_FALSE(config.dry_run);
    TEST_ASSERT_FALSE(config.recursive);
    assert_files_equal("tests/fixtures/expected/all_toc_prop.org",
                       "tests/fixtures/outputs/output_temp.org");
} // test_multiple_flags()

/**
 * @brief Tests invalid flags
 * @see parse_args()
 * */
void test_invalid_flag(void) {
    char *argv[] = {"md_to_org", "-w", "tests/fixtures/expected/all.md"};
    int argc = 3;
    Config config = {0};
    int result = parse_args(argc, argv, &config);
    TEST_ASSERT_EQUAL(1, result);
} // test_invalid_flag()

/**
 * @brief Tests missing input
 * @see parse_args()
 * @note Should print help
 * */
void test_no_input(void) {
    char *argv[] = {"md_to_org", "-t"};
    int argc = 2;
    Config config = {0};
    int result = parse_args(argc, argv, &config);
    TEST_ASSERT_EQUAL(1, result);
} // test_no_input()

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_no_flag_file);
    RUN_TEST(test_no_flag_dir);
    RUN_TEST(test_output_file_flag);
    RUN_TEST(test_output_dir_flag);
    RUN_TEST(test_no_output);
    RUN_TEST(test_dry_flag);
    RUN_TEST(test_dry_recursive_flag);
    RUN_TEST(test_toc_flag);
    RUN_TEST(test_props_flag);
    RUN_TEST(test_force_flag);
    RUN_TEST(test_help_flag);
    RUN_TEST(test_help_flag_no_path);
    RUN_TEST(test_recursive_flag);
    RUN_TEST(test_recursive_output_flag);
    RUN_TEST(test_multiple_flags);
    RUN_TEST(test_invalid_flag);
    RUN_TEST(test_no_input);

    return UNITY_END();
} // main()
