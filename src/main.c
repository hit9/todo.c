#include "buffer.h"
#include "todo.h"
#include "utils.h"
#include "parser.h"
#include "generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mcheck.h>
#define UNIT 1024

/* list all tasks */
void ls_tasks(todo_t *);

/* print single task */
void print_task(task_t *, int);


int
main(int argc, const char *argv[])
{
    mtrace();
    /* read todo.txt */
    FILE *fp;
    buf_t * buf;
    size_t ret;

    buf = buf_new(UNIT);

    //open file
    if (!(fp = fopen("todo.txt", "r"))) {
        printf("failed to open file 'todo.txt'");
        exit(1);
    }

    buf_grow(buf, UNIT);
    // read file
    while (
            (ret = fread(buf->data + buf->size, sizeof(uint8_t), buf->a_size - buf->size, fp)) > 0
    ) {
        buf->size += ret;
        buf_grow(buf, buf->size + UNIT);
    }

    fclose(fp);

    /* parse todo */
    todo_t *td = todo_new();
    unsigned int re = todo_parse(td, buf->data, buf->size);

    if (0 == re) {  // success parsed
        /* parse arguments */
        switch (argc) {
            case 1 :
                ls_tasks(td);
                break;
            case 2 :
            {
                char *p;
                int idx = (int) strtol(argv[1], &p, 10);  // try to convert to integer

                if (*p == '\0') {  // is integer like, idx > 0
                    task_t *tsk = todo_get(td, idx-1);
                    if (tsk)
                        print_task(tsk, idx);
                    else
                        printf("task '%d' not found", idx);
                }
                break;
            }
        }

        /* generate to str */
        buf_t *ob = buf_new(64);

        todo_generate(td, ob);

        if (!(fp=fopen("todo.txt", "w"))) {
            printf("failed to open 'todo.txt'");
        } else {
            if (fwrite(ob->data, sizeof(uint8_t), ob->size, fp) < 0 )
                printf("failed to write 'todo.txt'");
            fclose(fp);
        }
        buf_free(ob);
    }
    else
        printf("syntax error at line %d", re);

    /* clean up */
    todo_free(td);
    buf_free(buf);

    return 0;
}


void
ls_tasks(todo_t *td)
{

    task_t *t;
    int i;

    for (i=1, t=td->head; t; t=t->next, i++) {
        print_task(t, i);
    }
}


/* print single task, the id is 1, 2, 3... (start from 1) */
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

    uint8_t *clr_st = colored(t->state == done ? a : b, max_size, color);

    printf("%d. ", id);  // printf id
    printf("%s ", clr_st);  //printf task's state
    printf("%.*s\n", t->c_size, t->content);  //printf task's content

    /* free clr_st */
    free(clr_st);
}
