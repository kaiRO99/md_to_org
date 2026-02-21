#include "parse_line.h"
#include "states.h"
#include <climits>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Parses a line or mardown text and converts to org mode syntax.
 *
 * @param {char *} src  Line to parse
 * @param {FILE*} fout File to output to
 * */

void parse_line(char *src, FILE *fout) {

    // NOTE: Links
    regex_t link_regex, image_regex, table_regex, bold_regex, italic_regex,
        sthrough_regex, code_regex;

    regmatch_t link_m[4];

    // Pattern
    const char *link_pattern = "(!?)\\[([^]]+)\\]\\(([^)]+)\\)";
    regcomp(&link_regex, link_pattern, REG_EXTENDED);

    // Link
    if (regexec(&link_regex, src, 4, link_m, 0) == 0) {
        // full match

        // Get positions:
        int start = link_m[0].rm_so;
        int end = link_m[0].rm_so;
        int length = end - start;

        // Full match
        int match_len = link_m[0].rm_eo - link_m[0].rm_so;
        char *matched = malloc(match_len + 1);
        memcpy(matched, src + link_m[0].rm_so, match_len);
        matched[match_len] = '\0';
        // extract alt_text
        int alt_text_len = link_m[0].rm_eo - link_m[0].rm_so;
        char *alt_text = malloc(match_len + 1);
        memcpy(alt_text, src + link_m[0].rm_so, match_len);
        alt_text[alt_text_len] = '\0';
        // extract url
        int url_len = link_m[0].rm_eo - link_m[0].rm_so;
        char *url = malloc(match_len + 1);
        memcpy(url, src + link_m[0].rm_so, match_len);
        url[url_len] = '\0';

        char *new_link = malloc(alt_text_len + url_len + 7);
        sprintf(new_link, "[[%s][%s]]", alt_text, url);

        char *new_line = replace_substring(src, matched, new_link);

        src = new_line;

        free(new_line);
    }
    // TODO: Table
    regcomp(&table_regex, "^\\|(\\s*:?-+:?\\s*\\|)+$");
    if (regexec(&table_regex, src, 0, NULL, 0) == 0) {
        // the line is a table separator
        //
        int len = strlen(src);

        // get first and last | positions
        int first = -1, last = -1;
        for (int i = 0; i < len; i++) {
            if (src[i] == '|') {
                if (first == -1) {
                    first = i;
                } // if
                last = i;
            } // if
        } // for

        // replace | in between;
        for (int i = first + 1; i < last; i++) {
            if (src[i] == '|') {
                src[i] = '+';
            } // if
        } // for
    } // if

    // TODO: Bold, code, italics, strikethrough
    // BUG: anyhing with snake case? mathematical equations?
    // BUG: does not handle multiline emphasis
    /*
    ** Find nearest match of a pattern, moving cursor forward at each position
    */
    // Bold: **text** or __text__
    regcomp(&bold_regex, "\\*\\*([^*]+)\\*\\*|__([^_]+)__", REG_EXTENDED);
    // Italic: *text* or _text_ (but not ** or __)
    regcomp(&italic_regex, "\\*([^*]+)\\*|_([^_]+)_", REG_EXTENDED);
    // Strikethrough: ~text~
    regcomp(&sthrough_regex, "~([^~]+)~", REG_EXTENDED);
    // Inline code: `text`
    regcomp(&code_regex, "`([^`]+)`", REG_EXTENDED);

    const char *cursor = src;
    while (*cursor) {
        regmatch_t code_match[2], bold_match[2], italic_match[2],
            sthrough_match[2];

        int has_code = regexec(&code_regex, cursor, 2, code_match, 0) == 0;
        int has_bold = regexec(&bold_regex, cursor, 2, bold_match, 0) == 0;
        int has_italic =
            regexec(&italic_regex, cursor, 2, italic_match, 0) == 0;
        int has_sthrough =
            regexec(&sthrough_regex, cursor, 2, sthrough_match, 0) == 0;

        // find which comes first
        int code_start = has_code ? code_match[0].rm_so : INT_MAX;
        int bold_start = has_bold ? bold_match[0].rm_so : INT_MAX;
        int italic_start = has_italic ? italic_match[0].rm_so : INT_MAX;
        int sthrough_start = has_sthrough ? sthrough_match[0].rm_so : INT_MAX;

        if (!has_code && !has_sthrough && !has_bold && !has_italic)
            break;

        if (code_start < bold_start && code_start < italic_start &&
            code_start < sthrough_start) {
            // code is next
            // code_match[0].rm_so : statrt of match
            // code_match[0].rm_eo : one past last matching character
            src[code_match[0].rm_so] = '~';
            src[code_match[0].rm_eo - 1] = '~';

            cursor += code_match[0].rm_eo;
        } else if (bold_start < italic_start && bold_start < sthrough_start) {
            // bold is next

            // TODO: does org mode need to have spaces before *?
            // Remove one of the
            memmove(&src[bold_match[0].rm_so], &src[bold_match[0].rm_so + 1],
                    strlen(src) - 1);

            src[bold_match[0].rm_so + 1] = '*';
            memmove(&src[bold_match[0].rm_eo], &src[bold_match[0].rm_eo - 2],
                    strlen(src) - 1);

            src[bold_match[0].rm_eo - 1] = '*';

            cursor += bold_match[0].rm_eo;
        } else if (italic_start < sthrough_start) {
            // italic is next
            // * or _ to /
            src[italic_match[0].rm_so] = '/';
            src[italic_match[0].rm_eo - 1] = '/';

            cursor += italic_match[0].rm_eo;
        } else {
            // sthrough is next
            src[sthrough_match[0].rm_so] = '+';
            src[sthrough_match[0].rm_eo - 1] = '+';

            cursor += sthrough_match[0].rm_eo;
        } // if else

        // Output line
        fputs(src, fout);

    } // while

    regfree(&link_regex);
    regfree(&image_regex);
    regfree(&table_regex);
    regfree(&bold_regex);
    regfree(&italic_regex);
    regfree(&sthrough_regex);
    regfree(&code_regex);

} // parse_line
