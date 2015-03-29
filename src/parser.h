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

#ifndef PARSER_H
#define PARSER_H

#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "share.h"
#include "hbuf.h"
#include "todo.h"

typedef struct parser_result_st {
    todo_t *todo;
    size_t lineno;
    int error;
} parser_result_t;

/**
 * todo_t *todo = NULL;
 * parser_result_t *result = todo_parse(buf)
 *
 * if (result->error == TD_OK)
 *   todo = result->todo;
 * parser_result_free(result);
 * ...
 * todo_free(todo);
 */
parser_result_t *todo_parse(hbuf_t *);
void parser_result_free(parser_result_t *);

#endif
