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

static uint8_t *
skip_space(uint8_t *ch)
{
    while (*ch == ' ' || *ch == '\t')
        ch++;
    return ch;
}

void
parser_result_free(parser_result_t *parser_result)
{
    if (parser_result != NULL)
        free(parser_result);
}

parser_result_t *
todo_parse(hbuf_t *buf)
{
    assert(buf != NULL);

    parser_result_t *result = malloc(sizeof(parser_result_t));

    result->todo = NULL;
    result->lineno = 1;
    result->error = TD_OK;

    if (result == NULL)
        return NULL;  // ENOMEM

    result->todo = todo_new();

    if (result->todo == NULL) {
        result->error = TD_ENOMEM;
        return result;  // ENOMEM
    }

    if (buf->size == 0)
        return result;

    task_t *task = NULL;
    uint8_t *data = NULL;
    size_t size = 0;
    int state = undo;

    uint8_t *ch = buf->data;

    while (ch < buf->data + buf->size) {
        while (*(ch = skip_space(ch)) == '\n')
            (result->lineno)++ && ch++;

        if (ch >= buf->data + buf->size)
            break;

        if (*ch++ != '-') {
            result->error = TD_ESYNTAX;
            return result;
        }

        ch = skip_space(ch);

        if (*ch++ != '[') {
            result->error = TD_ESYNTAX;
            return result;
        }

        ch = skip_space(ch);

        switch (*ch) {
            case 'x':
                state = done;
                ch += 1;
                break;
            case ']':
                state = undo;
                break;
            default:
                result->error = TD_ESYNTAX;
                return result;
        }

        ch = skip_space(ch);

        if (*ch++ != ']') {
            result->error = TD_ESYNTAX;
            return result;
        }

        ch = skip_space(ch);

        data = ch;

        while (*ch++ != '\n');

        size = ch - data - 1;  // exclude '\n'

        if ((task = task_new(state, data, size)) == NULL) {
            result->error = TD_ENOMEM;
            return result;
        }

        todo_push(result->todo, task);

        result->lineno += 1;  // count '\n'
    }
    return result;
}
