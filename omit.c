/*
 * Programming assignment 1
 *
 * Removes C comments from a source file and writes result to stdout.
 * Comments are replaced with a single space.
 * Handles single and double quote sequences for character and string literals.
 * Comments and quoted sequences can span multiple lines.
 *
 * Error handling:
 *      Prints starting line for unterminated comments or quote sequences.
 */

#include <stdio.h>
#include <stdlib.h>
#include "omit.h"

#define STATE_ERROR "Error: unhandled state\n"
#define QUOTE_ERROR "Error: line %d: unterminated quote(%c)\n"
#define COMMENT_ERROR "Error: line %d: unterminated comment\n"

int
main(void)
{
    int c;                          // Input character
    int linecnt = 1;                // Tracks number of lines
    int startline = 1;              // Start line for comment/quote

    enum typestate state = START;   // Initial state of DFA

    /*
     * Process a single char at a time and update current state with
     * corresponding state subroutine until EOF is reached.
     */
    while ((c = getchar()) != EOF) {
        switch (state) {
        case START:
            state = START_sub(c);

            // If we are leaving START state, update startline.
            if (state != START)
                startline = linecnt;
            break;

        case SINGLE_QUOTE:
            state = SINGLE_QUOTE_sub(c);
            break;

        case DOUBLE_QUOTE:
            state = DOUBLE_QUOTE_sub(c);
            break;

        case SINGLE_QUOTE_ESC:
            state = SINGLE_QUOTE_ESC_sub(c);
            break;

        case DOUBLE_QUOTE_ESC:
            state = DOUBLE_QUOTE_ESC_sub(c);
            break;

        case SINGLE_SLASH:
            state = SINGLE_SLASH_sub(c);
            break;

        case DOUBLE_SLASH:
            state = DOUBLE_SLASH_sub(c);
            break;

        case SLASH_STAR:
            state = SLASH_STAR_sub(c);
            break;

        case STAR_SLASH:
            state = STAR_SLASH_sub(c);
            break;

        default:
            /*
             * This code should never be reached unless a state is added and
             * not properly handled.
             */
            fprintf(stderr, STATE_ERROR);
            return EXIT_FAILURE;
        }

        // We've reached a newline -- update linecnt.
        if (c == '\n')
            linecnt++;
    }

    /*
     * All done -- check for error states and print error message accordingly.
     * If we ended in a '/' state, print that '/' since we skipped over it.
     */
    switch (state) {
    case SINGLE_SLASH:
        putchar('/');
        break;

    case SINGLE_QUOTE:
    case SINGLE_QUOTE_ESC:
        fprintf(stderr, QUOTE_ERROR, startline, '\'');
        return EXIT_FAILURE;

    case DOUBLE_QUOTE:
    case DOUBLE_QUOTE_ESC:
        fprintf(stderr, QUOTE_ERROR, startline, '"');
        return EXIT_FAILURE;

    case SLASH_STAR:
    case STAR_SLASH:
        fprintf(stderr, COMMENT_ERROR, startline);
        return EXIT_FAILURE;

    default:
        break;
    }

    // We didn't end in an error state -- return from program successfully.
    return EXIT_SUCCESS;
}
