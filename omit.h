#ifndef _OMIT_H
#define _OMIT_H

// Enumerate definition for the different possible states.
enum typestate {START, SINGLE_QUOTE, SINGLE_QUOTE_ESC, DOUBLE_QUOTE,
                DOUBLE_QUOTE_ESC, SINGLE_SLASH, DOUBLE_SLASH, SLASH_STAR,
                STAR_SLASH};

// State subroutine prototypes.
enum typestate START_sub(int);
enum typestate SINGLE_QUOTE_sub(int);
enum typestate SINGLE_QUOTE_ESC_sub(int);
enum typestate DOUBLE_QUOTE_sub(int);
enum typestate DOUBLE_QUOTE_ESC_sub(int);
enum typestate SINGLE_SLASH_sub(int);
enum typestate DOUBLE_SLASH_sub(int);
enum typestate SLASH_STAR_sub(int);
enum typestate STAR_SLASH_sub(int);

#endif
