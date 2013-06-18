#include "todo.h"
#include <stdlib.h>

/* return an empty todo */
todo_t *
todo_new()
{
    todo_t *td = (todo_t *)malloc(sizeof(todo_t));

    if (td) {
        td->head = 0;
    }

    return td;
}


/* return todo's task count */
size_t
todo_size(todo_t *td)
{
    size_t i;
    task_t *t;

    for (i=0, t=td->head; t; t=t->next, i++);

    return i;
}


/* return an task */
task_t *
task_new(uint8_t *content, size_t content_size, int state)
{
    task_t *tk = (task_t *)malloc(sizeof(task_t));

    if (tk) {
        tk->state = state;
        tk->content = content;
        tk->c_size = content_size;
        tk->next = 0;
    }

    return tk;
}


/* append a task to todo list */
void
todo_append(todo_t *td, task_t *tk)
{
    task_t *t = td->head; /* the head node */

    if (t) {
        for (; t->next; t=t->next); /* goto the last node */
        t->next = tk;  /* append task `tk` */
    } else  /* else, initialize the head task */
        td->head = tk;
}


/* free all tasks and the todo */
void
todo_free(todo_t *td)
{
    task_t *t = td->head, *next;

    while (t) {
        /* record the next */
        next = t->next;
        /* free current node */
        free(t);
        /* go to next node */
        t = next;
    }  /* until t == 0 , yet the last one */

    /* free todo */
    free(td);
}


/* get task by position */
task_t *
todo_get(todo_t *td, int position)
{
    int i;
    task_t *t;

    for (t=td->head, i=0; t; t=t->next, i++) {
        if (position == i)
            return t;
    }

    return 0;  // not found!
}

/* pop task, return 0 is ok*/
int
todo_pop(todo_t *td, task_t *tk)
{
    task_t *head = td->head, *t;

    if (tk == head){ //if pop node is the head node
        td->head = head->next;
        free(head);
        return 0;
    }

    for (t=head; t->next != tk; t = t->next); //else goto the pop node's pre node

    t->next = tk->next;
    free(tk);
}

void
todo_clear(todo_t *td)
{
    task_t *t = td->head, *next;

    // free all nodes
    while (t) {
        next = t->next;
        free(t);
        t = next;
    }

    // point head to null
    td->head = 0;
}
