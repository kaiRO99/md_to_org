/**
 * @file parse_line.h
 * @brief Parses a line for mardown syntax, converting to Org-mode.
 * @author Kai Ryall Ota
 * @date Feb 2026
 * */
#ifndef PARSE_LINE_H_
#define PARSE_LINE_H_

/**
 * @brief Parses a line or mardown text and converts to Org-mode syntax.
 *
 * @param{char*} src  Line to parse
 *
 * @returns{char*} Parsed line
 * */
char *parse_line(char *src);

#endif // PARSE_LINE_H_
