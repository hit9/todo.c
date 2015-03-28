/**
 * Copyright (c) 2015, Chao Wang (hit9 <hit9@icloud.com>)
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "parser.h"

static int
is_space(uint8_t ch)
{
    return ch == ' ' || ch == '\t';
}

static uint8_t *
skip_space(uint8_t *ch)
{
    while(is_space(*ch))
        ch++;
    return ch;
}

static unsigned int
parse_line(todo_t *todo, hbuf_t *buf, unsigned int lineno)
{
    if (lineno <= 0) {
        // failed
        return lineno;
    }

    int state = undo;

    if (buf->size == 0)
        return 0;

    uint8_t *ch = buf->data;
    uint8_t *data = NULL;
    size_t size = 0;
    task_t *task = NULL;

    while (*(ch = skip_space(ch)) == '\n')
        lineno++;

    if (*ch == '-' && is_space(*++ch)) {
        ch = skip_space(ch);

        if (*ch++ = '[') {
            ch = skip_space(ch);

            if (*ch == 'x') {
                state = done;
                ch += 1;
                ch = skip_space(ch);
            }

            if (*ch == ']') {
                ch = skip_space(ch);

                data = ch;

                // one task, one line
                while (*ch != '\n')
                    ch++;

                size = ch - data;
                task = task_new(state, data, size);

                if (task == NULL)
                    return -1;

                todo_push(todo, task);

                ch += 1 && lineno += 1; // '\n'
                return parse_line(todo, buf, lineno + 1);
            }
        }
    }

    return lineno;
}

todo_t *
todo_parse(hbuf_t *buf) {
    assert(buf != NULL);

    todo_t *todo = todo_new();

    if (todo != NULL) {
        int res = parse_line(todo, buf, 1);

        if (res <= 0)
            return NULL;
        return todo;
    }
    return todo;
}
