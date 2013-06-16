#include "todo.h"
#include <stdlib.h>

todo_t *
todo_new()  /* return an empty todo */
{
    todo_t *td = (todo_t *)malloc(sizeof(todo_t));

    if (td) {
        td->head = 0;
    }

    return td;
}


size_t
todo_size(todo_t *td) /* return todo's task count */
{
    size_t i;
    task_t *t;

    for (i=0, t=td->head; t; t=t->next, i++);

    return i;
}


task_t *
task_new(uint8_t *content, size_t content_size) /* return an task */
{
    task_t *tk = (task_t *)malloc(sizeof(task_t));

    if (tk) {
        tk->state = undo;
        tk->content = content;
        tk->c_size = content_size;
        tk->next = 0;
    }

    return tk;
}


void
todo_append(todo_t *td, task_t *tk) /* append a task to todo list */
{
    task_t *t = td->head; /* the head node */

    if (t) {
        for (; t->next; t=t->next); /* goto the last node */
        t->next = tk;  /* append task `tk` */
    } else  /* else, initialize the head task */
        td->head = tk;
}


void
todo_free(todo_t *td)  /* free all tasks and the todo */
{
    task_t *t = td->head, *next;

    while (t){
        /* record the next */
        next = t->next;
        /* free current node */
        free(t->content);
        free(t);
        /* go to next node */
        t = next;
    }  /* until t == 0 , yet the last one */

    /* free todo */
    free(td);
}
