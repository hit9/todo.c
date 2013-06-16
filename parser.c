#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
    task_t *tk;
    int state;
    uint8_t *content;
    size_t c_size;


    if (!size) return;

    /* starting parser */

    uint8_t *c=str;

    while (_is_space(*c)) c++; /* allow spaces */

    /* starts with '-' */
    assert(*c == '-'  && _is_space(*++c));

    while (_is_space(*c)) c++;

    /* starts state */
    assert(*c++ == '[');

    while (_is_space(*c)) c++;

    if (*c == 'x') {
        state = done;
        c++;
        while (_is_space(*c)) c++;
    }

    assert(*c++ == ']');

    while (_is_space(*c)) c++;

    content = c;

    while (*c != '\n') c++; /* one task, one line */

    c_size = c - content;

    /* append this task to todo */
    tk = task_new(content, c_size, state);
    todo_append(td, tk);

    /* parse the rest str */
    c++;
    todo_parse(td, c, size - (c-str));
}
