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
#define VERSION "0.1.0"

/* show usage */
void show_help();

/* list all tasks */
void ls_tasks(todo_t *);

/* print single task */
void print_task(task_t *, int);

/* print all undo tasks */
void ls_undo_tasks(todo_t *);

typedef enum {
    RE_OK = 0,
    RE_IO_ERROR = 1,      /* errors in IO*/
    RE_ID_ERROR = 2,      /* Invalid id */
    RE_SYNTAX_ERROR = 3,  /* Syntax error */
} return_codes;


/* main: cli interface */
int
main(int argc, const char *argv[])
{
    /* memery trace for memory leaks detection */
    mtrace();

    /* main's return code */
    int rt = RE_OK;

    /*
     * try read file 'todo.txt'
     */

    FILE *fp;
    buf_t * buf;
    size_t ret;

    buf = buf_new(UNIT);

    if (!(fp = fopen("todo.txt", "r"))) {
        printf("failed to open file 'todo.txt'\n");
        exit(RE_IO_ERROR);
    }

    /* read file 'todo.txt' */

    buf_grow(buf, UNIT);

    while (
            (ret = fread(buf->data + buf->size, sizeof(uint8_t), buf->a_size - buf->size, fp)) > 0
    ) {
        buf->size += ret;
        buf_grow(buf, buf->size + UNIT);
    }

    fclose(fp);

    /*
     * parse read content to todo struct
     */

    todo_t *td = todo_new();

    /* re: parsing result , 0: success */
    unsigned int error_line = todo_parse(td, buf->data, buf->size);

    if (0 == error_line) {  // success parsed

        /*
         * parse arguments
         */
        switch (argc) {

            case 1:
                /* no extra arguments, list all undone tasks */
                ls_undo_tasks(td);
                break;

            case 2:
                if (0 == strcmp(argv[1], "clear")) { /* clear todo */
                    todo_clear(td);
                    break;
                } else if(0 == strcmp(argv[1], "-a") ||
                        0 == strcmp(argv[1], "--all")) { /* list all tasks */
                    ls_tasks(td);
                    break;
                } else if (0 == strcmp(argv[1], "-h") ||
                        0 == strcmp(argv[1], "--help")) { /* show help message */
                    show_help();
                    break;
                } else if (0 == strcmp(argv[1], "-v") ||
                        0 == strcmp(argv[1], "--version")) { /* show version */
                    printf("todo version: %s\n", VERSION);
                    break;
                }
            case 3:
            {
                /* try to convert to integer */
                char *p;
                int idx = (int) strtol(argv[1], &p, 10);

                if (*p == '\0') {  /* argv[1] is integer like */

                    task_t *tsk = todo_get(td, idx-1);

                    if (tsk) {

                        if (argc == 2){  /* display this task */
                            print_task(tsk, idx);
                        } else if (argc == 3) {
                            /* check task done */
                            if (0 == strcmp(argv[2], "done")) tsk->state = done;
                            /* check task undo */
                            else if (0 == strcmp(argv[2], "undo")) tsk->state = undo;
                            /* remove task */
                            else if (0 == strcmp(argv[2], "remove")) todo_pop(td, tsk);
                            /* add this a new task */
                            else
                                goto add_task;
                        }

                    } else {  /* failed to get task, tsk is 0*/
                        printf("task '%d' not found\n", idx);
                        rt = RE_ID_ERROR;
                    }

                } else
                    goto add_task;
                break;
            }
        /* add new task */
        add_task:
            default:
            {
                buf_t *c_buf = buf_new(64);

                /* join arguments with spaces */
                space_join((uint8_t **)(argv + 1), argc - 1, c_buf);

                task_t *new_tsk = task_new(c_buf->data, c_buf->size, undo);

                todo_append(td, new_tsk);
                /* free this buffer struct but not buf_free it.
                 * the buf's data is freed when we free todo
                 */
                free(c_buf);
                break;
            }
        }

        /*
         * generate to str
         */
        buf_t *ob = buf_new(64);

        todo_generate(td, ob);

        /* write to file todo.txt */

        if (!(fp = fopen("todo.txt", "w"))) {

            printf("failed to open 'todo.txt'\n");
            rt = RE_IO_ERROR;

        } else {

            if (fwrite(ob->data, sizeof(uint8_t), ob->size, fp) < 0 ) {
                printf("failed to write 'todo.txt'\n");
                rt = RE_IO_ERROR;
            }

            fclose(fp);
        }

        buf_free(ob);

    } else {
        /* error_line > 0, got syntax error in parsing process */
        printf("syntax error at line %d\n", error_line);
        rt = RE_SYNTAX_ERROR;
    }

    /* clean up allocated memory */
    todo_free(td);
    buf_free(buf);

    return rt;
}

/* show help message */
void
show_help()
{
    printf("Usage:\n");
    printf("  todo [-h|-v|-a]\n");
    printf("  todo (<id> [done|undo|remove])|<task>...\n");
    printf("Examples:\n");
    printf("  add a task       -  todo Go shopping\n");
    printf("  check a task     -  todo 1 done\n");
    printf("  undo a task      -  todo 1 undo\n");
    printf("  remove a task    -  todo 1 remove\n");
    printf("  list undo tasks  -  todo\n");
    printf("  list all tasks   -  todo --all\n");
    printf("  clear all tasks  -  todo clear\n");
    printf("Have found a bug? Please let me know: https://github.com/hit9/todo.c/issues\n");
}

/* list all tasks */
void
ls_tasks(todo_t *td)
{

    task_t *t;
    int i;

    for (i = 1, t = td->head; t; t = t->next, i++) {
        print_task(t, i);
    }
}

/* list undo tasks */
void
ls_undo_tasks(todo_t *td)
{

    task_t *t;
    int i;

    for (i = 1, t = td->head; t; t = t->next, i++) {
        if (t->state == undo)
            print_task(t, i);
    }
}


/* print single task, the id is 1, 2, 3... (start from 1) */
void
print_task(task_t *t, int id)
{
    /* get state color. done: green; undo: red */
    int color = t->state == done ? green : red;

    uint8_t *a = "✓";  /* sign: done */
    uint8_t *b = "✖";  /* sign: undo */

    size_t a_size = strlen(a);
    size_t b_size = strlen(b);
    size_t max_size = a_size > b_size ? a_size : b_size;

    uint8_t *clr_st = colored(t->state == done ? a : b, max_size, color);

    printf("%d. ", id);
    printf("%s ", clr_st);
    printf("%.*s\n", t->c_size, t->content);

    free(clr_st);
}
