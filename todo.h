#ifndef TODO_H
#define TODO_H

#include <stdint.h>
#include <stddef.h>

/* data structures: task, todo */

typedef enum {
    done = 1,
    undo = 0,
} task_state; /* task's state */

typedef struct task {
    uint8_t state;
    uint8_t *content;  /* task's content: string */
    size_t c_size;  /* content string size */
    struct task *next; /* next task */
} task_t;

/* todo is just a list of tasks */
typedef struct todo {
    task_t *head; /* head task */
} todo_t;

task_t *task_new(uint8_t *, size_t);  /* return an **empty** task */
todo_t *todo_new();  /* return an **empty** todo */
void todo_append(todo_t *, task_t *);  /* append task to todo list's tail */
size_t todo_size(todo_t *);  /* return todo list's length */
task_t *todo_get(todo_t *, size_t);  /* return task by its position in todo list */

#endif
