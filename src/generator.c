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

#include "generator.h"

hbuf_t *
todo_generate(todo_t *todo)
{
    assert(todo != NULL);

    hbuf_t *buf = hbuf_new(TODO_BUF_UNIT);

    if (buf == NULL)
        return NULL;

    task_t *task = todo->head;

    while (task != NULL) {
        if (hbuf_sprintf(buf, "- [%c] %.*s\n",
                task->state == done ? 'x' : ' ',
                task->data->size,
                task->data->data) != HBUF_OK)
            return NULL;

        task = task->next;
    }
    return buf;
}
