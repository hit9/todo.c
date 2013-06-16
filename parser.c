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
    return chr == ' ' || chr == '\t' || chr == '\n';
}


/*
 * parse string to todo struct.
 * - [ ] this is a undo task
 * - [x] this is a done task
 */
void
todo_parse(todo_t *td, uint8_t *str, size_t size)
{
    int state;
    uint8_t *content;
    size_t c_size;


    if (!size) return;  /* nothing to parse */

    /* starting parser */

    uint8_t *c=str;

    while (_is_space(*c)) c++; /* allow spaces */

    /* starts with '-' */
    if (*c == '-' && _is_space(*++c)) {

        while (_is_space(*c)) c++;

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
                todo_parse(td, c, size - (c-str));
            }

        }
    }
}
