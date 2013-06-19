/*
 * Copyright (c) 2013, hit9
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright notice,
 *       this list of conditions and the following disclaimer in the documentation
 *       and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * helper function
 */

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
