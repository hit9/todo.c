#include "parser.h"
#include <stdio.h>
#include <stdlib.h>

/* ***********************
 * helper functions
 * ************************/

/* test if a character is space */
static int
_is_space(uint8_t chr)
{
    return chr == ' ' || chr == '\t';
}


/*
 * parse string to todo struct.
 * - [ ] this is a undo task
 * - [x] this is a done task
 * return 0 for success,  return others for error line number
 */
static unsigned int
todo_parse_line(todo_t *td, uint8_t *str, size_t size, unsigned int line_no)
{
    int state = undo;  // default: undo
    uint8_t *content;
    size_t c_size;


    if (!size) return 0;  /* nothing to parse */

    /* starting parser */

    uint8_t *c=str;

    // skip empty
    while (*c == '\n' ||
            *c == ' ' ||
            *c == '\t') {
        if (*c == '\n') line_no++;
        c++;
    }

    if (*c == '\0') return 0;  // the end of string

    /* starts with '-' */
    if (*c == '-' && _is_space(*++c)) {

        while (_is_space(*c)) c++; /* allow spaces */

        if (*c++ == '[') {

            while (_is_space(*c)) c++;

            /* starting to parse state */
            if (*c == 'x') {
                state = done;
                c++;
                while (_is_space(*c)) c++;
            }

            if (*c++ == ']') {
                /* starting to parse content */

                while (_is_space(*c)) c++;

                content = c;

                /* one task, one line */
                while (*c != '\n') c++;

                c_size = c - content;

                /* append this task to todo */
                todo_append(td, task_new(content, c_size, state));

                /* parse the rest str */
                c++;  //skip '\n'

                return todo_parse_line(td, c, size - (c-str), line_no+1);
            }

        }
    }

    return line_no;
}


unsigned int
todo_parse(todo_t *td, uint8_t *str, size_t size)
{
    return todo_parse_line(td, str, size, 1); /* parse from line 1 */
}
