/**
 * @file states.h
 * @brief States to keep track of multiline markdown syntax.
 * @author Kai Ryall Ota
 * @date Feb 2026
 * */
#ifndef STATES_H_
#define STATES_H_

/**
 * @brief General states to track what syntax is available
 * */
typedef enum {
    DEFAULT,
    PROPERTIES,
    CODE_BLOCK,
    QUOTE,
} State;

/**
 * @brief Sub states to track states within the general states
 * */
typedef enum {
    NONE,
    PROP_LIST,
    BOLD,
    ITALIC,
    STRIKETHROUGH,
    CODE,
    TABLE,
} SubState;

/**
 * @brief Section states to track if a section has been inserted
 * */
typedef enum {
    DONE,
    MISSING,
} SectionState;

/**
 * @brief Header level to track when to insert table of contents
 * */
typedef enum {
    H0,
    H1,
    H2,
    H3,
    H4,
    H5,
    H6,
} HeaderState;

#endif // STATES_H_
