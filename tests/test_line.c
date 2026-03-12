/**
 * @file test_line.c
 * @brief Tests for line conversion from markdown to Org-mode.
 * @author Kai Ryall Ota
 * @date Feb 2026
 *
 * @note Tests:
 *       - Links
 *       - Bold
 *       - Italics
 *       - Inline code
 *       - Table separator
 *       - Strikethrough
 *       - All in a line except table separator
 * */
#include "../include/parse_line.h"
#include "../tests/unity/unity.h"
#include "../tests/unity/unity_internals.h"
#include <stdlib.h>

/**
 * @brief Setup before each test
 * */
void setUp(void) {
    // Setup before each test
}

/**
 * @brief Cleanup after each test
 * */
void tearDown(void) {
    // Cleanup after each test
}

/**
 * @brief Test for links.
 * @note Tests:
 *       - Valid url link
 *       - Valid image link
 *       - Invalid url link
 *       - Invalid image link
 *
 * @see parse_line()
 * */
void test_link(void) {
    char *result;

    // Valid case 1
    char *valid_input_c1 = "words [alt text](url) text";
    result = parse_line(valid_input_c1);
    TEST_ASSERT_EQUAL_STRING("words [[url][alt text]] text", result);
    free(result);

    // Valid case 2
    char *valid_input_c2 = "words ![alt text](imagepath) text";
    result = parse_line(valid_input_c2);
    TEST_ASSERT_EQUAL_STRING("words [[imagepath][alt text]] text", result);
    free(result);

    // Invalid: should not change
    char *invalid_input_c1 = "text [text] (text) text";
    result = parse_line(invalid_input_c1);
    TEST_ASSERT_EQUAL_STRING(invalid_input_c1, result);
    free(result);

    // Invalid: should not change
    char *invalid_input_c2 = "text ![text] (text) text";
    result = parse_line(invalid_input_c2);
    TEST_ASSERT_EQUAL_STRING(invalid_input_c2, result);
    free(result);
} // test_link()

/**
 * @brief Test for bold
 * @note Tests:
 *       - Valid: **text**
 *       - Valid: __text__
 *       - Valid: text**text**text
 *       - Invalid
 * @see parse_line()
 * */
void test_bold(void) {
    char *result;
    // Valid case 1
    char *valid_input_c1 = "text **text** text";
    result = parse_line(valid_input_c1);
    TEST_ASSERT_EQUAL_STRING("text *text* text", result);
    free(result);

    // Valid case 2
    char *valid_input_c2 = "text __text__ text";
    result = parse_line(valid_input_c2);
    TEST_ASSERT_EQUAL_STRING("text *text* text", result);
    free(result);

    // Valid case 3
    char *valid_input_c3 = "text**text**text";
    result = parse_line(valid_input_c3);
    TEST_ASSERT_EQUAL_STRING("text*text*text", result);
    free(result);

    // Invalid: should not change
    char *invalid_input_c1 = "text**text text";
    result = parse_line(invalid_input_c1);
    TEST_ASSERT_EQUAL_STRING(invalid_input_c1, result);
    free(result);
} // test_bold()

/**
 * @brief Test for italic
 * @note Tests:
 *       - Valid: *text*
 *       - Valid: _text_
 *       - Invalid
 * @see parse_line()
 * */
void test_italic(void) {
    char *result;

    // Valid case 1
    char *valid_input_c1 = "text *text* text";
    result = parse_line(valid_input_c1);
    TEST_ASSERT_EQUAL_STRING("text /text/ text", result);
    free(result);

    // Valid case 2
    char *valid_input_c2 = "text _text_ text";
    result = parse_line(valid_input_c2);
    TEST_ASSERT_EQUAL_STRING("text /text/ text", result);
    free(result);

    // Invalid: should not change
    char *invalid_input_c1 = "text*text text";
    result = parse_line(invalid_input_c1);
    TEST_ASSERT_EQUAL_STRING(invalid_input_c1, result);
    free(result);

} // test_italic()

/**
 * @brief Test for inline code
 * @note Tests:
 *       - Valid
 *       - Valid w/ emphasis: `text*text*`
 *       - Invalid
 * @see parse_line()
 * */
void test_code(void) {
    char *result;

    // Valid case 1
    char *valid_input_c1 = "text `code` text";
    result = parse_line(valid_input_c1);
    TEST_ASSERT_EQUAL_STRING("text ~code~ text", result);
    free(result);

    // Valid case 2
    char *valid_input_c2 = "text `code*code*` text";
    result = parse_line(valid_input_c2);
    TEST_ASSERT_EQUAL_STRING("text ~code*code*~ text", result);
    free(result);

    // Invalid: should not change
    char *invalid_input_c1 = "text`text text";
    result = parse_line(invalid_input_c1);
    TEST_ASSERT_EQUAL_STRING(invalid_input_c1, result);
    free(result);

} // test_code()

/**
 * @brief Test for table separator
 * @note Tests:
 *       - Valid: | --- | --- |
 *       - Valid w/ alignment: | :--- |
 *       - Valid w/ alignment: | :---: |
 *       - valid: | ------- | -------- |
 *       - Invalid: | -- | -- |
 * @see parse_line()
 * */
void test_table(void) {
    char *result;

    // Valid case 1
    char *valid_input_c1 = "| --- | --- |\n";
    result = parse_line(valid_input_c1);
    TEST_ASSERT_EQUAL_STRING("| --- + --- |\n", result);
    free(result);

    // Valid case 2
    char *valid_input_c2 = "| :--- | ---: | --- |";
    result = parse_line(valid_input_c2);
    TEST_ASSERT_EQUAL_STRING("|  --- + ---  + --- |", result);
    free(result);

    // Valid case 3
    char *valid_input_c3 = "| :---: | :---: |";
    result = parse_line(valid_input_c3);
    TEST_ASSERT_EQUAL_STRING("|  ---  +  ---  |", result);
    free(result);

    // Valid case 4
    char *valid_input_c4 = "| ------- | ------- |";
    result = parse_line(valid_input_c4);
    TEST_ASSERT_EQUAL_STRING("| ------- + ------- |", result);
    free(result);

    // Valid case 5
    char *valid_input_c5 = "|-------|-------|";
    result = parse_line(valid_input_c5);
    TEST_ASSERT_EQUAL_STRING("|-------+-------|", result);
    free(result);

    // Invalid: should not change
    char *invalid_input_c1 = "| -- | -- |";
    result = parse_line(invalid_input_c1);
    TEST_ASSERT_EQUAL_STRING(invalid_input_c1, result);
    free(result);
} // test_table

/**
 * @brief Test for strikethrough
 * @note Tests:
 *       - valid: ~text~
 *       - invalid: ~text
 * @see parse_line()
 * */
void test_sthrough(void) {
    char *result;

    // Valid case 1
    char *valid_input_c1 = "text ~text~ text";
    result = parse_line(valid_input_c1);
    TEST_ASSERT_EQUAL_STRING("text +text+ text", result);
    free(result);

    // Invalid: should not change
    char *invalid_input_c1 = "text~text text";
    result = parse_line(invalid_input_c1);
    TEST_ASSERT_EQUAL_STRING(invalid_input_c1, result);
    free(result);

} // test_sthrough

/**
 * @brief Test all in a line
 * @note Tests:
 *       - Valid, separated: old, italic, code, strikethrough,
 *       - TODO: Valid: nested emphasis
 * @see parse_line()
 * */
void test_all(void) {
    char *result;

    // Valid case 1
    char *valid_input_c1 =
        "text **bold text** text *italic text* text `code code` text ~sthrough "
        "text~ text [link alt](link url) text";
    result = parse_line(valid_input_c1);
    TEST_ASSERT_EQUAL_STRING(
        "text *bold text* text /italic text/ text ~code code~ text +sthrough "
        "text+ text [[link url][link alt]] text",
        result);
    free(result);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_link);
    RUN_TEST(test_code);
    RUN_TEST(test_bold);
    RUN_TEST(test_italic);
    RUN_TEST(test_table);
    RUN_TEST(test_sthrough);
    RUN_TEST(test_all);

    return UNITY_END();
} // main()
