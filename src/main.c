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

/* print all undo tasks */
void ls_undo_tasks(todo_t *);


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
            case 1:
                ls_undo_tasks(td);
                break;
            case 2:
                if (0 == strcmp(argv[1], "clear")) {
                    // clear todo
                    todo_clear(td);
                    break;
                } else if(0 == strcmp(argv[1], "-a") ||
                        0 == strcmp(argv[1], "--all")) {
                    ls_tasks(td);
                    break;
                }
            case 3:
            {
                // check if the  argv[1] is a integer
                char *p;
                int idx = (int) strtol(argv[1], &p, 10);  // try to convert to integer

                if (*p == '\0') {  // is integer like, idx > 0
                    // fetch this task out
                    task_t *tsk = todo_get(td, idx-1);

                    if (tsk) { // the task is avliable

                        if (argc == 2)  // means query task
                            print_task(tsk, idx);
                        else if (argc == 3) { // check or remove the task
                            /* check task done */
                            if (0 == strcmp(argv[2], "done")) tsk->state = done;
                            /* check task undo */
                            else if (0 == strcmp(argv[2], "undo")) tsk->state = undo;
                            /* remove task */
                            else if (0 == strcmp(argv[2], "remove")) todo_pop(td, tsk);
                            /* as a task */
                            else
                                goto add_task;
                        }

                    }
                    else
                        printf("task '%d' not found", idx);

                }
                else
                    goto add_task;
                break;
            }
            // add as todo
        add_task:
            default:
            {
                buf_t *buf = buf_new(64);  // new a buffer to store arguments
                space_join(
                        (uint8_t **)(argv+1), // throw away the first argv
                        argc-1,  // the arr long is now argc-1
                        buf); //join argvs to buffer with " "
                task_t *new_tsk = task_new(buf->data, buf->size, undo);
                todo_append(td, new_tsk);
                break;
            }
        }

        /* generate to str */
        buf_t *ob = buf_new(64);

        todo_generate(td, ob);

        // write to file
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

void
ls_undo_tasks(todo_t *td)
{

    task_t *t;
    int i;

    for (i=1, t=td->head; t; t=t->next, i++) {
        if (t->state == undo)
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
