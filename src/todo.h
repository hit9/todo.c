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


#ifndef TODO_H
#define TODO_H

#include <stdint.h>
#include <stddef.h>

/* task's state: done & undo */
typedef enum {
    done = 1,
    undo = 0,
} task_state;

typedef struct task {
    uint8_t state;
    uint8_t *content;
    size_t c_size;  /* content string size */
    struct task *next; /* next task */
} task_t;

/* todo is just a list of tasks */
typedef struct todo {
    task_t *head; /* head task */
} todo_t;

/*
 * api
 */

/* return an **empty** task */
task_t *task_new(uint8_t *, size_t, int);

/* return an **empty** todo */
todo_t *todo_new();

/* append task to todo list's tail */
void todo_append(todo_t *, task_t *);

/* return todo list's length */
size_t todo_size(todo_t *);

/* return task by its position in todo list */
task_t *todo_get(todo_t *, int);

/* pop task out */
int todo_pop(todo_t *, task_t *);

/* clear todo list */
void
todo_clear(todo_t *td);

/* remove done tasks from todo list */
void
todo_cleanup(todo_t *td);

#endif
