#ifndef STATES_H_
#define STATES_H_

/*
** General states
** Used to track what emphasis/syntax is available
*/
typedef enum {
    DEFAULT,
    PROPERTIES,
    CODE_BLOCK,
    QUOTE,
} State;

/*
** Sub states
** Used to track states within the general state
*/
typedef enum {
    NONE,
    PROP_LIST,
    BOLD,
    ITALIC,
    STRIKETHROUGH,
    CODE,
    TABLE,
} SubState;

/*
** Section states
** Used to track if a section has been inserted
*/
typedef enum {
    DONE,
    MISSING,
} SectionState;

/*
 *
 * Header Level
 * Used to track when to insert table of contents */
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
