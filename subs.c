#include <stdio.h>
#include "omit.h"

/*
 * Start state
 *
 * Checks for the start to a character literal, a string literal, and a comment.
 * If no start to these sequences is found, simply prints the current character
 * and keeps the START state.
 */
enum typestate
START_sub(int c)
{
    switch (c) {
    case '\'':
        putchar('\'');
        return SINGLE_QUOTE;

    case '"':
        putchar('"');
        return DOUBLE_QUOTE;

    case '/':
        return SINGLE_SLASH;
}

    putchar(c);
    return START;
}

/*
 * Handle character literals
 *
 * Checks for the end of the character literal and for an escaped character. If
 * none of these are found, simply prints the current character and keeps the
 * SINGLE_QUOTE state.
 */
enum typestate
SINGLE_QUOTE_sub(int c)
{
    switch (c) {
    case '\'':
        putchar('\'');
        return START;

    case '\\':
        putchar('\\');
        return SINGLE_QUOTE_ESC;
    }

    putchar(c);
    return SINGLE_QUOTE;
}

/*
 * Handle string literals
 *
 * Checks for the end of the string literal and for an escaped character. If
 * none of these are found, simply prints the current character and keeps the
 * DOUBLE_QUOTE state.
 */
enum typestate
DOUBLE_QUOTE_sub(int c)
{
    switch (c) {
    case '"':
        putchar('"');
        return START;

    case '\\':
        putchar('\\');
        return DOUBLE_QUOTE_ESC;
    }

    putchar(c);
    return DOUBLE_QUOTE;
}


/*
 * Handle character literal escaped character
 *
 * Saw a backslash in a char literal, so we simply ignore the special meaning of
 * the following char while printing it.
 */
enum typestate
SINGLE_QUOTE_ESC_sub(int c)
{
    putchar(c);
    return SINGLE_QUOTE;
}

/*
 * Handle string literal escaped character
 *
 * Saw a backslash in a string literal, so we simply ignore the special meaning
 * of the following char while printing it.
 */
enum typestate
DOUBLE_QUOTE_ESC_sub(int c)
{
    putchar(c);
    return DOUBLE_QUOTE;
}

/*
 * Handle single slash
 *
 * Checks for the start of a C++ style comment, C style comment, and for the
 * beginning of a character or string literal. If we have a comment, prints a
 * single space to replace first line of comment with single space. If we have
 * a single quote, prints skipped slash + single quote and enters SINGLE_QUOTE
 * state. If we have a double quote, prints skipped slash + double quote and
 * enters DOUBLE_QUOTE state. In any other case, we print the skipped / + the
 * next character and enter START state.
 */
enum typestate
SINGLE_SLASH_sub(int c)
{
    switch (c) {
    case '/':
        putchar(' ');
        return DOUBLE_SLASH;

    case '*':
        putchar(' ');
        return SLASH_STAR;

    case '\'':
        printf("/'");
        return SINGLE_QUOTE;

    case '"':
        printf("/\"");
        return DOUBLE_QUOTE;
    }

    printf("/%c", c);
    return START;
}

/*
 * Handle double slash
 *
 * Saw two slashes (at least) -- we are in a C++ style comment. If there is a
 * newline character, we print newline and go back to start state, otherwise,
 * keep DOUBLE_SLASH state.
 */
enum typestate
DOUBLE_SLASH_sub(int c)
{
    switch (c) {
    case '\n':
        putchar('\n');
        return START;
    }

    return DOUBLE_SLASH;
}

/*
 * Handle slash start
 *
 * Saw a slash* -- looking for another *. If we see another star, we potentially
 * have a C comment terminator, so go to STAR_SLASH state. If there is a
 * newline, prints the newline and stays in SLASH_STAR state.
 */
enum typestate
SLASH_STAR_sub(int c)
{
    switch (c) {
    case '*':
        return STAR_SLASH;

    case '\n':
        putchar('\n');
    }

    return SLASH_STAR;
}

/*
 * Handle slash star
 *
 * Saw a slash** -- looking for closing slash to C-style comment. If we have
 * another slash, the comment is terminated so we go back to START state. If
 * we have another *, we stay inside the STAR_SLASH state. If There is a newline
 * character, we print the newline and go back to the SLASH_STAR state. Any
 * other character, and we simply go back to the SLASH_STAR state.
 */
enum typestate
STAR_SLASH_sub(int c)
{
    switch (c) {
    case '/':
        return START;

    case '*':
        return STAR_SLASH;

    case '\n':
        putchar('\n');
        return SLASH_STAR;
    }

    return SLASH_STAR;
}
