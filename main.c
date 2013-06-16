#include "buffer.h"
#include "todo.h"
#include "utils.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define UNIT 10

/* list all tasks */
void ls_tasks(todo_t *);

/* print single task */
void print_task(task_t *, int);


int main(int argc, const char *argv[])
{
    FILE *fp;

    if (NULL == (fp = fopen("test1", "r")))
        exit(1);

    buf_t * buf = buf_new(UNIT);
    size_t ret;
    buf_grow(buf, UNIT);

    while ((ret = fread(buf->data+buf->size, 1, buf->a_size-buf->size, fp)) > 0) {
        buf->size += ret;
        buf_grow(buf, buf->size+UNIT);
    }

    todo_t *td = todo_new();
    todo_parse(td, buf->data, buf->size);
    ls_tasks(td);
    buf_free(buf);
    return 0;
}


void
ls_tasks(todo_t *td)
{

    task_t *t;
    int i;

    for (i=0, t=td->head; t; t=t->next, i++) {
        print_task(t, i);
    }
}


void
print_task(task_t *t, int id)
{

    int color = t->state == done ? green : red;  // get state color

    /* get state mark */
    uint8_t *a = "✓";
    uint8_t *b = "✖";

    size_t a_size = strlen(a);
    size_t b_size = strlen(b);
    size_t max_size = a_size > b_size ? a_size : b_size;

    printf("%d. ", id);  // printf id
    printf("%s ", colored(t->state == done ? a : b, max_size, color));  //printf task's state
    printf("%.*s\n", t->c_size, t->content);  //printf task's content
}
