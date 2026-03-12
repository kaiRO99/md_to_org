/* #include "test_utils.h" */
#include "../include/replace_all.h"
#include "../include/replace_substring.h"
#include "../include/starts_with.h"
#include "unity.h"
#include <stdlib.h>

void setUp(void) {
    // Setup before each test
}

void tearDown(void) {
    // Cleanup after each test
}

/**
 * Tests for replace_substring()
 * Cases:
 * - String contains substring
 * - String does not contain substring
 * */
void test_replace_substring(void) {
    char *result;
    char *input = "``` testing";

    result = replace_substring(input, "```", "#+BEGIN_SRC");
    TEST_ASSERT_EQUAL_STRING("#+BEGIN_SRC testing", result);

    free(result);

    // No match
    result = replace_substring(input, "###", "***");
    TEST_ASSERT_EQUAL_STRING("``` testing", result);

    free(result);
}
/**
 * Tests for starts_with()
 * Cases:
 * - String does start with substring
 * - String does not start with substring
 * */
void test_starts_with(void) {
    int result;
    char *input = "> testing ";

    // valid
    result = starts_with(input, ">");
    TEST_ASSERT_EQUAL_INT(0, result);

    // No match
    result = starts_with(input, "#");
    TEST_ASSERT_NOT_EQUAL_UINT8(0, result);
}

/**
 * Tests for replace_all()
 * Cases:
 * - Valid
 * - Invalid: partial matches
 * - Invalid: no matches
 * */
void test_replace_all(void) {
    char *result;
    char *input = "~asdf~asdf~asdf~asdf~";

    // Valid
    result = replace_all(input, "asdf", "#$");
    TEST_ASSERT_EQUAL_STRING("~#$~#$~#$~#$~", result);
    free(result);

    // Partial match: no change
    result = replace_all(input, "asdfgh", "123");
    TEST_ASSERT_EQUAL_STRING(input, result);
    free(result);

    // Partial match: no change
    result = replace_all(input, "acdf", "123");
    TEST_ASSERT_EQUAL_STRING(input, result);
    free(result);
} // test_replace_all

int main(void) {
    UNITY_BEGIN();

    // string replacement tests
    RUN_TEST(test_replace_substring);
    RUN_TEST(test_starts_with);
    RUN_TEST(test_replace_all);

    return UNITY_END();
}
