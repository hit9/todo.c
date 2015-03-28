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

#include "todo.h"

/**
 * New task.
 */
task_t *
task_new(int state, uint8_t *data, size_t size)
{
    task_t *task = malloc(sizeof(task_t));

    if (task != NULL) {
        task->state = state;
        task->next = NULL;

        hbuf_t *buf = hbuf_new(BUF_UNIT);

        if (!buf)
            return NULL;

        if (hbuf_put(buf, data, size) != HBUF_OK)
            return NULL;

        task->data = buf;
    }
    return task;
}

/**
 * Free task.
 */
void
task_free(task_t *task)
{
    if (task != NULL) {
        if (task->data != NULL)
            hbuf_free(task->data);
        free(task);
    }
}

/**
 * New todo.
 */
todo_t *
todo_new()
{
    todo_t *todo = malloc(sizeof(todo_t));

    if (todo != NULL) {
        todo->head = NULL;
    }
    return todo;
}

/**
 * Get todo size.
 */
size_t
todo_size(todo_t *todo)
{
    assert(todo != NULL);

    size_t size = 0;
    task_t *task = todo->head;

    while (task != NULL) {
        size += 1;
        task = task->next;
    }
    return size;
}

/**
 * Clear todo.
 */
void
todo_clear(todo_t *todo)
{
    assert(todo != NULL);

    task_t *task = todo->head;
    task_t *next;

    while (task != NULL) {
        next = task->next;
        task_free(task);
        task = next;
    }

    todo->head = NULL;
}

/**
 * Free todo.
 */
void
todo_free(todo_t *todo)
{
    if (todo != NULL) {
        todo_clear(todo);
        free(todo);
    }
}

/**
 * Push a task at last of todo.
 */
void
todo_push(todo_t *todo, task_t *task)
{
    assert(task->next == NULL);

    task_t *seek = todo->head;

    if (seek != NULL) {
        while (seek->next != NULL)
            seek = seek->next;
        seek->next = task;
    } else {
        todo->head = task;
    }
}

/**
 * Get task by index.
 */
task_t *
todo_get(todo_t *todo, size_t idx)
{
    assert(todo != NULL);

    task_t *task = todo->head;

    while (task != NULL) {
        if (idx == 0)
            return task;
        task = task->next;
        idx -= 1;
    }

    return NULL;
}

/**
 * Pop a task by index.
 */
int
todo_pop(todo_t *todo, size_t idx)
{
    assert(todo != NULL);

    task_t *task = todo->head;
    task_t *prev = NULL;

    while (task != NULL && idx != 0) {
        prev = task;
        task = task->next;
        idx -= 1;
    }

    if (task == NULL)
        return TD_ENOTFOUND;

    if (task == todo->head) {
        assert(prev == NULL);
        todo->head = task->next;
    } else {
        assert(prev != NULL);
        prev->next = task->next;
    }

    task_free(task);
    return TD_OK;
}

/**
 * Clean done tasks.
 */
void
todo_clean(todo_t *todo)
{
    task_t *task = todo->head;
    task_t *prev = NULL;

    while (task != NULL) {
        if (task->state == done) {
            if (task == todo->head) {
                assert(prev == NULL);
                todo->head = task->next;
                task_free(task);
                task = todo->head;
            } else {
                assert(prev != NULL);
                prev->next = task->next;
                task_free(task);
                task = task->next;
            }
        } else {
            prev = task;
            task = task->next;
        }
    }
}
