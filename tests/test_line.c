/**
 * Tests for parse_line.c
 * Cases:
 * - Links
 * - TODO: Bold
 * - Italics
 * - TODO: Inline code
 * - TODO: Table separator
 * - TODO: Strikethrough
 * - TODO: All
 *
 * @author Kai Ryall Ota
 * */
#include "../include/parse_line.h"
#include "unity.h"
#include <stdlib.h>
#include <string.h>

void setUp(void) {
    // Setup before each test
}

void tearDown(void) {
    // Cleanup after each test
}

/**
 * Test for links
 * cases:
 * - Valid url link
 * - Valid image link
 * - Invalid url link
 * - Invalid image link
 * */
void test_link(void) {
    char *result;

    // Valid case 1
    char *valid_input_c1 = "words [alt text](url) text";
    result = parse_line(valid_input_c1);
    TEST_ASSERT_EQUAL_STRING("words [[alt text][url]] text", result);
    free(result);

    // Valid case 2
    char *valid_input_c2 = "words ![alt text](imagepath) text";
    result = parse_line(valid_input_c2);
    TEST_ASSERT_EQUAL_STRING("words [[alt text][imagepath]] text", result);
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
} // test_link

/**
 * Test for bold
 * cases:
 * - Valid: **text**
 * - Valid: __text__
 * - Valid: text**text**text
 * - Invalid
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
} // test_bold

/**
 * Test for italic
 * cases:
 * - Valid: *text*
 * - Valid: _text_
 * - Invalid
 * */
void test_italic(void) {
    //
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

} // test_italic

/**
 * Test for inline code
 * cases:
 * - Valid
 * - Valid w/ emphasis: `text*text*`
 * - Invalid
 * */
void test_code(void) {
    //
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

} // test_code

/**
 * Test for table separator
 * cases:
 * - Valid: | --- | --- |
 * - Valid w/ alignment: | :--- |
 * - Valid w/ alignment: | :---: |
 * - valid: | ------- | -------- |
 * - Invalid: | -- | -- |
 * -  */
void test_table(void) {
    //
    char *result;

    // Valid case 1
    char *valid_input_c1 = "| --- | --- |\n";
    result = parse_line(valid_input_c1);
    TEST_ASSERT_EQUAL_STRING("| --- + --- |\n", result);
    free(result);

    // Valid case 2
    char *valid_input_c2 = "| :--- | ---: |";
    result = parse_line(valid_input_c2);
    TEST_ASSERT_EQUAL_STRING("| :--- + ---: |", result);
    free(result);

    // Valid case 3
    char *valid_input_c3 = "| :---: | :---: |";
    result = parse_line(valid_input_c3);
    TEST_ASSERT_EQUAL_STRING("| :---: + :---: |", result);
    free(result);

    // Valid case 4
    char *valid_input_c4 = "| ------- | ------- |";
    result = parse_line(valid_input_c4);
    TEST_ASSERT_EQUAL_STRING("| ------- + ------- |", result);
    free(result);

    // Invalid: should not change
    char *invalid_input_c1 = "| -- | -- |";
    result = parse_line(invalid_input_c1);
    TEST_ASSERT_EQUAL_STRING(invalid_input_c1, result);
    free(result);
} // test_table

/**
 * Test for strikethrough
 * cases:
 * - valid: ~text~
 * - invalid: ~text
 * */
void test_sthrough(void) {
    //
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
 * Test for mixed
 * */
int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_link);
    RUN_TEST(test_code);
    RUN_TEST(test_bold);
    RUN_TEST(test_italic);
    RUN_TEST(test_table);
    RUN_TEST(test_sthrough);

    return UNITY_END();
}
