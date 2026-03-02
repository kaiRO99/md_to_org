/**
 * @file parse_line.c
 * @brief Parses a line for markdown syntax, converting to Org-mode
 * @author Kai Ryall Ota
 * @date Feb 2026
 * */
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
 * @brief Parses a line or mardown text and converts to Org-mode syntax.
 *
 * @param{char*} src  Line to parse
 *
 * @returns{char*} Parsed line
 * */
char *parse_line(char *src) {

    char *original_src = src;
    regex_t link_regex, table_regex, bold_regex, italic_regex, sthrough_regex,
        code_regex;
    regmatch_t table_match[3];
    size_t new_cursor;
    // Table separator
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
        char *no_pipe_substring = replace_all(content, "|", "+");
        char *new_substring = replace_all(no_pipe_substring, ":", " ");

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

    // Link pattern
    const char *link_pattern = "(!?)\\[([^]]+)\\]\\(([^)]+)\\)";
    regcomp(&link_regex, link_pattern, REG_EXTENDED);
    // Bold, code, italics, strikethrough
    // Bold: **text** or __text__
    regcomp(&bold_regex, "\\*\\*([^*]+)\\*\\*|__([^_]+)__", REG_EXTENDED);
    // Italic: *text* or _text_
    regcomp(&italic_regex, "\\*([^*]+)\\*|_([^_]+)_", REG_EXTENDED);
    // Strikethrough: ~text~
    regcomp(&sthrough_regex, "~([^~]+)~", REG_EXTENDED);
    // Inline code: `text`
    regcomp(&code_regex, "`([^`]+)`", REG_EXTENDED);

    const char *cursor = src;
    while (*cursor) {
        regmatch_t link_match[4], code_match[3], bold_match[4], italic_match[4],
            sthrough_match[3];

        int has_code = regexec(&code_regex, cursor, 3, code_match, 0) == 0;
        int has_bold = regexec(&bold_regex, cursor, 4, bold_match, 0) == 0;
        int has_italic =
            regexec(&italic_regex, cursor, 4, italic_match, 0) == 0;
        int has_sthrough =
            regexec(&sthrough_regex, cursor, 3, sthrough_match, 0) == 0;
        int has_link = regexec(&link_regex, cursor, 4, link_match, 0) == 0;
        // find which comes first
        int code_start = has_code ? code_match[0].rm_so : INT_MAX;
        int bold_start = has_bold ? bold_match[0].rm_so : INT_MAX;
        int italic_start = has_italic ? italic_match[0].rm_so : INT_MAX;
        int sthrough_start = has_sthrough ? sthrough_match[0].rm_so : INT_MAX;
        int link_start = has_link ? link_match[0].rm_so : INT_MAX;

        if (!has_code && !has_sthrough && !has_bold && !has_italic &&
            !has_link) { // Output line as is
            break;
        } // if
        // Link
        if (link_start < code_start && link_start < bold_start &&
            link_start < italic_start && link_start < sthrough_start) {
            // entire match
            int match_len = link_match[0].rm_eo - link_match[0].rm_so;
            char *matched = malloc(match_len + 1);
            memcpy(matched, cursor + link_match[0].rm_so, match_len);
            matched[match_len] = '\0';

            // extract alt_text
            int alt_text_len = link_match[2].rm_eo - link_match[2].rm_so;
            char *alt_text = malloc(alt_text_len + 1);
            memcpy(alt_text, cursor + link_match[2].rm_so, alt_text_len);
            alt_text[alt_text_len] = '\0';

            // extract url
            int url_len = link_match[3].rm_eo - link_match[3].rm_so;
            char *url = malloc(url_len + 1);
            memcpy(url, cursor + link_match[3].rm_so, url_len);
            url[url_len] = '\0';

            char *new_link =
                malloc(alt_text_len + url_len + 7); // extra padding
            sprintf(new_link, "[[%s][%s]]", url, alt_text);

            char *new_cursor_str = replace_substring(cursor, matched, new_link);

            // rebuild full string
            size_t pre_len = cursor - src;
            char *new_src = malloc(pre_len + strlen(new_cursor_str) + 1);
            memcpy(new_src, src, pre_len);
            strcpy(new_src + pre_len, new_cursor_str);

            free(new_cursor_str);

            if (src != original_src) {
                free(src);
            } // if
            src = new_src;
            new_cursor = pre_len + link_match[0].rm_so + strlen(new_link);
            cursor = src + new_cursor;

            free(new_link);
            free(matched);
            free(alt_text);
            free(url);
        } else if (code_start < bold_start && code_start < italic_start &&
                   code_start < sthrough_start) {
            // Code is next
            int content_len = code_match[1].rm_eo - code_match[1].rm_so;
            int content_offset = code_match[1].rm_so;

            char *content = malloc(content_len + 1);
            memcpy(content, cursor + content_offset, content_len);
            content[content_len] = '\0';

            // build string to replace
            int match_len = code_match[0].rm_eo - code_match[0].rm_so;
            char *matched = malloc(match_len + 1);
            memcpy(matched, cursor + code_match[0].rm_so, match_len);
            matched[match_len] = '\0';

            // build replacement string
            char *converted = malloc(content_len + 3); // +3 for \0
            sprintf(converted, "~%s~", content);

            char *new_cursor_str =
                replace_substring(cursor, matched, converted);

            // rebuild
            size_t pre_len = cursor - src;
            char *new_src = malloc(pre_len + strlen(new_cursor_str) + 1);
            memcpy(new_src, src, pre_len);
            strcpy(new_src + pre_len, new_cursor_str);
            free(new_cursor_str);
            if (src != original_src) {
                free(src);
            } // if

            src = new_src;
            new_cursor = pre_len + code_match[0].rm_so + strlen(converted);
            cursor = src + new_cursor;

            free(content);
            free(matched);
            free(converted);

        } else if (bold_start < italic_start && bold_start < sthrough_start) {
            // extract content
            int content_len;
            int content_offset;
            // Location in src
            /* size_t match_src = (cursor - src) + bold_match[0].rm_so; */
            /* // build matched from position in src */
            /* int match_len = bold_match[0].rm_eo - bold_match[0].rm_so; */
            /* char *matched = malloc(match_len + 1); */
            /* memcpy(matched, src + match_src, match_len); */
            /* matched[match_len] = '\0'; */

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

            char *new_cursor_str =
                replace_substring(cursor, matched, converted);

            // build full string
            size_t pre_len = cursor - src;
            char *new_src = malloc(pre_len + strlen(new_cursor_str) + 1);
            memcpy(new_src, src, pre_len);
            strcpy(new_src + pre_len, new_cursor_str);

            free(new_cursor_str);

            if (src != original_src) {
                free(src);
            } // if

            src = new_src;
            new_cursor = pre_len + bold_match[0].rm_so + strlen(converted);
            cursor = src + new_cursor;

            free(content);
            free(matched);
            free(converted);

        } else if (italic_start < sthrough_start) {
            // italic is next
            int content_len;
            int content_offset;
            // Location in src
            /* size_t match_src = (cursor - src) + italic_match[0].rm_so; */
            /* // build matched from position in src */
            /* int match_len = italic_match[0].rm_eo - italic_match[0].rm_so; */
            /* char *matched = malloc(match_len + 1); */
            /* memcpy(matched, src + match_src, match_len); */
            /* matched[match_len] = '\0'; */

            if (italic_match[1].rm_so != -1) {
                // *text*
                content_len = italic_match[1].rm_eo - italic_match[1].rm_so;
                content_offset = italic_match[1].rm_so;

            } else if (italic_match[2].rm_so != -1) {
                // _text_
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

            char *new_cursor_str =
                replace_substring(cursor, matched, converted);

            // rebuild string
            size_t pre_len = cursor - src;
            char *new_src = malloc(pre_len + strlen(new_cursor_str) + 1);
            memcpy(new_src, src, pre_len);
            strcpy(new_src + pre_len, new_cursor_str);

            free(new_cursor_str);

            if (src != original_src) {
                free(src);
            } // if

            src = new_src;
            new_cursor = pre_len + italic_match[0].rm_so + strlen(converted);
            cursor = src + new_cursor;

            free(content);
            free(matched);
            free(converted);

        } else {
            // sthrough is next
            int content_len = sthrough_match[1].rm_eo - sthrough_match[1].rm_so;
            int content_offset = sthrough_match[1].rm_so;

            char *content = malloc(content_len + 1);
            memcpy(content, cursor + content_offset, content_len);
            content[content_len] = '\0';

            // build string to replace
            int match_len = sthrough_match[0].rm_eo - sthrough_match[0].rm_so;
            char *matched = malloc(match_len + 1);
            memcpy(matched, cursor + sthrough_match[0].rm_so, match_len);
            matched[match_len] = '\0';

            // build replacement string
            char *converted = malloc(content_len + 3); // +3 for \0
            sprintf(converted, "+%s+", content);

            char *new_cursor_str =
                replace_substring(cursor, matched, converted);

            // rebuild
            size_t pre_len = cursor - src;
            char *new_src = malloc(pre_len + strlen(new_cursor_str) + 1);
            memcpy(new_src, src, pre_len);
            strcpy(new_src + pre_len, new_cursor_str);

            free(new_cursor_str);

            if (src != original_src) {
                free(src);
            } // if

            src = new_src;
            new_cursor = pre_len + sthrough_match[0].rm_so + strlen(converted);
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
