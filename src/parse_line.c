#define _POSIX_C_SOURCE 200809L

#include "parse_line.h"
#include "replace_all.h"
#include "replace_substring.h"
#include <limits.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Parses a line or mardown text and converts to org mode syntax.
 *
 * @param {char*} src  Line to parse
 *
 * @returns {char*} Parsed line
 * */

char *parse_line(char *src) {
    char *original_src = src;
    // NOTE: Links
    regex_t link_regex, table_regex, bold_regex, italic_regex, sthrough_regex,
        code_regex;

    regmatch_t link_match[4], table_match[3];

    size_t new_cursor;
    // Pattern
    const char *link_pattern = "(!?)\\[([^]]+)\\]\\(([^)]+)\\)";
    regcomp(&link_regex, link_pattern, REG_EXTENDED);

    // Link
    if (regexec(&link_regex, src, 4, link_match, 0) == 0) {
        // full match

        // Get positions:
        /* int start = link_match[0].rm_so; */
        /* int end = link_match[0].rm_so; */
        /* int length = end - start; */

        // Full match
        int match_len = link_match[0].rm_eo - link_match[0].rm_so;
        char *matched = malloc(match_len + 1);
        memcpy(matched, src + link_match[0].rm_so, match_len);
        matched[match_len] = '\0';

        // extract alt_text
        int alt_text_len = link_match[2].rm_eo - link_match[2].rm_so;
        char *alt_text = malloc(alt_text_len + 1);
        memcpy(alt_text, src + link_match[2].rm_so, alt_text_len);
        alt_text[alt_text_len] = '\0';
        // extract url
        int url_len = link_match[3].rm_eo - link_match[3].rm_so;
        char *url = malloc(url_len + 1);
        memcpy(url, src + link_match[3].rm_so, url_len);
        url[url_len] = '\0';

        char *new_link = malloc(alt_text_len + url_len + 15); // extra padding
        sprintf(new_link, "[[%s][%s]]", alt_text, url);
        char *new_line = replace_substring(src, matched, new_link);
        if (src != original_src) {
            free(src);
        }
        src = new_line;

        free(new_link);
        free(matched);
        free(alt_text);
        free(url);
    }
    // Table
    regcomp(&table_regex,
            "^\\|[[:space:]]*((:?-{3,}:?[[:space:]]*\\|[[:space:]]*)+:?-{3,}:?)"
            "[[:space:]]*\\|[[:space:]]*$",
            REG_EXTENDED);
    if (regexec(&table_regex, src, 3, table_match, 0) == 0) {
        // extract content
        int content_len = table_match[1].rm_eo - table_match[1].rm_so;
        int content_offset = table_match[1].rm_so;

        char *content = malloc(content_len + 1);
        memcpy(content, src + content_offset, content_len);
        content[content_len] = '\0';

        // replace all | in between;
        char *new_substring = replace_all(content, "|", "+");

        // rebuild string
        int pre_len = table_match[1].rm_so;
        int post_len = strlen(src) - table_match[1].rm_eo;
        int new_sub_len = strlen(new_substring);
        int new_len = pre_len + new_sub_len + post_len;

        char *new_string = malloc(new_len + 1);

        memcpy(new_string, src, pre_len);
        memcpy(new_string + pre_len, new_substring, new_sub_len);
        memcpy(new_string + pre_len + new_sub_len, src + table_match[1].rm_eo,
               post_len);
        new_string[new_len] = '\0';

        src = new_string;
        free(content);
        free(new_substring);
    } // if

    // Bold, code, italics, strikethrough
    // BUG: anyhing with snake case? mathematical equations?
    // BUG: does not handle multiline emphasis
    /*
    ** Find nearest match of a pattern, moving cursor forward at each position
    */
    // Bold: **text** or __text__

    regcomp(&bold_regex, "\\*\\*([^*]+)\\*\\*|__([^_]+)__", REG_EXTENDED);
    /* regcomp(&bold_regex, "(?:\\*\\*|__)([^*_]+)(?:\\*\\*|__)", REG_EXTENDED);
     */
    // Italic: *text* or _text_ (but not ** or __)
    regcomp(&italic_regex, "\\*([^*]+)\\*|_([^_]+)_", REG_EXTENDED);
    // Strikethrough: ~text~
    regcomp(&sthrough_regex, "~([^~]+)~", REG_EXTENDED);
    // Inline code: `text`
    regcomp(&code_regex, "`([^`]+)`", REG_EXTENDED);

    const char *cursor = src;
    while (*cursor) {
        regmatch_t code_match[3], bold_match[4], italic_match[4],
            sthrough_match[3];

        int has_code = regexec(&code_regex, cursor, 3, code_match, 0) == 0;
        int has_bold = regexec(&bold_regex, cursor, 4, bold_match, 0) == 0;
        int has_italic =
            regexec(&italic_regex, cursor, 4, italic_match, 0) == 0;
        int has_sthrough =
            regexec(&sthrough_regex, cursor, 3, sthrough_match, 0) == 0;

        // find which comes first
        int code_start = has_code ? code_match[0].rm_so : INT_MAX;
        int bold_start = has_bold ? bold_match[0].rm_so : INT_MAX;
        int italic_start = has_italic ? italic_match[0].rm_so : INT_MAX;
        int sthrough_start = has_sthrough ? sthrough_match[0].rm_so : INT_MAX;

        if (!has_code && !has_sthrough && !has_bold &&
            !has_italic) { // Output line as is
            break;
        } // if
        if (code_start < bold_start && code_start < italic_start &&
            code_start < sthrough_start) {
            // code is next
            int content_len = code_match[1].rm_eo - code_match[1].rm_so;
            char *content = malloc(content_len + 1);

            memcpy(content, cursor + code_match[1].rm_so, content_len);
            content[content_len] = '\0';
            // build string to replace
            int match_len = code_match[0].rm_eo - code_match[0].rm_so;
            char *matched = malloc(match_len + 1);
            memcpy(matched, cursor + code_match[0].rm_so, match_len);
            matched[match_len] = '\0';

            // build replacement string
            char *converted = malloc(content_len + 3); // +3 for \0
            sprintf(converted, "~%s~", content);
            new_cursor =
                (cursor - src) + code_match[0].rm_so + strlen(converted);

            char *new_src = replace_substring(src, matched, converted);
            if (src != original_src) {
                free(src);
            } // if

            src = new_src;
            cursor = src + new_cursor;

            free(content);
            free(matched);
            free(converted);

        } else if (bold_start < italic_start && bold_start < sthrough_start) {
            // extract content
            int content_len;
            int content_offset;
            if (bold_match[1].rm_so != -1) {
                // **text**
                content_len = bold_match[1].rm_eo - bold_match[1].rm_so;
                content_offset = bold_match[1].rm_so;
            } else if (bold_match[2].rm_so != -1) {
                // __text__
                content_len = bold_match[2].rm_eo - bold_match[2].rm_so;
                content_offset = bold_match[2].rm_so;
            } else {
                cursor++;
                continue;
            } // if else

            char *content = malloc(content_len + 1);
            memcpy(content, cursor + content_offset, content_len);
            content[content_len] = '\0';

            // build string to replace
            int match_len = bold_match[0].rm_eo - bold_match[0].rm_so;
            char *matched = malloc(match_len + 1);
            memcpy(matched, cursor + bold_match[0].rm_so, match_len);
            matched[match_len] = '\0';

            // build replacement string
            char *converted = malloc(content_len + 3); // +3 for \0
            sprintf(converted, "*%s*", content);
            new_cursor =
                (cursor - src) + bold_match[0].rm_so + strlen(converted);

            char *new_src = replace_substring(src, matched, converted);
            if (src != original_src) {
                free(src);
            } // if

            src = new_src;
            cursor = src + new_cursor;

            free(content);
            free(matched);
            free(converted);

            /* // TODO: does org mode need to have spaces before *? */
        } else if (italic_start < sthrough_start) {
            // italic is next
            int content_len;
            int content_offset;
            if (italic_match[1].rm_so != -1) {
                // **text**
                content_len = italic_match[1].rm_eo - italic_match[1].rm_so;
                content_offset = italic_match[1].rm_so;

            } else if (italic_match[2].rm_so != -1) {
                // __text__
                content_len = italic_match[2].rm_eo - italic_match[2].rm_so;
                content_offset = italic_match[2].rm_so;
            } else {
                cursor++;
                continue;
            } // if else

            char *content = malloc(content_len + 1);
            memcpy(content, cursor + content_offset, content_len);
            content[content_len] = '\0';

            // build string to replace
            int match_len = italic_match[0].rm_eo - italic_match[0].rm_so;
            char *matched = malloc(match_len + 1);
            memcpy(matched, cursor + italic_match[0].rm_so, match_len);
            matched[match_len] = '\0';

            // build replacement string
            char *converted = malloc(content_len + 3); // +3 for \0
            sprintf(converted, "/%s/", content);
            new_cursor =
                (cursor - src) + italic_match[0].rm_so + strlen(converted);

            char *new_src = replace_substring(src, matched, converted);
            if (src != original_src) {
                free(src);
            } // if

            src = new_src;

            cursor = src + new_cursor;

            free(content);
            free(matched);
            free(converted);

        } else {
            // sthrough is next
            int content_len = sthrough_match[1].rm_eo - sthrough_match[1].rm_so;
            char *content = malloc(content_len + 1);
            memcpy(content, cursor + sthrough_match[1].rm_so, content_len);
            content[content_len] = '\0';

            // build string to replace
            int match_len = sthrough_match[0].rm_eo - sthrough_match[0].rm_so;
            char *matched = malloc(match_len + 1);
            memcpy(matched, cursor + sthrough_match[0].rm_so, match_len);
            matched[match_len] = '\0';

            // build replacement string
            char *converted = malloc(content_len + 3); // +3 for \0
            sprintf(converted, "+%s+", content);
            new_cursor =
                (cursor - src) + sthrough_match[0].rm_so + strlen(converted);

            char *new_src = replace_substring(src, matched, converted);
            if (src != original_src) {
                free(src);
            } // if

            src = new_src;
            cursor = src + new_cursor;

            free(content);
            free(matched);
            free(converted);

        } // if else
    } // while

    regfree(&link_regex);
    regfree(&table_regex);
    regfree(&bold_regex);
    regfree(&italic_regex);
    regfree(&sthrough_regex);
    regfree(&code_regex);

    char *result = strdup(src);
    if (src != original_src) {
        free(src);
    } // if
    return result;
} // parse_line
