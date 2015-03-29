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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wordexp.h>

#include "hbuf.h"
#include "todo.h"
#include "utils.h"
#include "parser.h"
#include "generator.h"

#ifdef __linux
#include <mcheck.h>
#endif

#define TD_VERSION "0.2.4"

#define TD_EMSG_NOMEM       "no memory"
#define TD_EMSG_IOR         "error to read %s"
#define TD_EMSG_IOW         "error to write %s"
#define TD_EMSG_SYNTAX      "syntax error at line %lu"
#define TD_EMSG_NOTFOUND    "task %lu not found"
#define TD_EMSG_INVALIDIDX  "invalid task index %d"

void td_emsg(const char *, ...);
void td_exit(int);
void td_help(void);
void td_version(void);
hbuf_t *td_try_txt();
hbuf_t *td_try_read(hbuf_t *);
todo_t *td_try_parse(hbuf_t *);
void td_try_write(hbuf_t *, todo_t *);
void td_task_print(task_t *, size_t);
void td_ls_all(todo_t *);
void td_ls_undo(todo_t *);
void td_try_idx(todo_t *, int);

int main(int argc, const char *argv[])
{

#ifdef __linux
    mtrace();
#endif

    hbuf_t *path = td_try_txt();
    todo_t *todo = td_try_parse(path);

    int error = TD_OK;

    switch (argc) {
        case 1:
            goto _ls_undo;
        case 2:
            if (0 == strcmp(argv[1], "-h") ||
                    0 == strcmp(argv[1], "--help"))
                goto _help;
            else if (0 == strcmp(argv[1], "-v") ||
                    0 == strcmp(argv[1], "--version"))
                goto _version;
            else if (0 == strcmp(argv[1], "-a") ||
                    0 == strcmp(argv[1], "--all"))
                goto _ls_all;
            else if (0 == strcmp(argv[1], "clear"))
                goto _clear;
            else if (0 == strcmp(argv[1], "cleanup") ||
                    0 == strcmp(argv[1], "clean"))
                goto _cleanup;
            else if (is_int_like(argv[1]))
                goto _idx;
            else
                goto _add;
        case 3:
            if (is_int_like(argv[1]))
                goto _idx;
            else
                goto _add;
        default:
            goto _add;
    }

_help:
    td_help();
    goto _exit;

_version:
    td_version();
    goto _exit;

_clear:
    todo_clear(todo);
    td_try_write(path, todo);
    goto _exit;

_cleanup:
    todo_clean(todo);
    td_try_write(path, todo);
    goto _exit;

_ls_all:
    td_ls_all(todo);
    goto _exit;

_ls_undo:
    td_ls_undo(todo);
    goto _exit;

_idx:
{
    int idx = str2int(argv[1]);

    if (idx < 1) {
        error = TD_EINVALIDIDX;
        td_emsg(TD_EMSG_INVALIDIDX, idx);
        goto _exit;
    }

    task_t *task = todo_get(todo, (size_t)(idx - 1));

    if (task == NULL) {
        error = TD_ENOTFOUND;
        td_emsg(TD_EMSG_NOTFOUND, idx);
        goto _exit;
    }

    if (argc == 2) {
        td_task_print(task, idx);
        goto _exit;
    } else if (argc == 3) {
        if (0 == strcmp(argv[2], "done"))
            goto _done;
        else if (0 == strcmp(argv[2], "undo"))
            goto _undo;
        else if (0 == strcmp(argv[2], "remove") ||
                0 == strcmp(argv[2], "rm"))
            goto _remove;
        else
            goto _help;
    }

_done:
    task->state = done;
    td_try_write(path, todo);
    goto _exit;

_undo:
    task->state = undo;
    td_try_write(path, todo);
    goto _exit;

_remove:
    todo_pop(todo, idx - 1);
    td_try_write(path, todo);
    goto _exit;
}

_add:
{
    hbuf_t *buf = hbuf_new(BUF_UNIT);

    int idx;

    for (idx = 0; idx < argc; idx++) {
        if (hbuf_puts(buf, (char *)argv[idx]) != HBUF_OK ||
                hbuf_puts(buf, " ") != HBUF_OK) {
            error = TD_ENOMEM;
            td_emsg(TD_EMSG_NOMEM);
            goto _exit;
        }
    }

    task_t *task = task_new(undo, buf->data, buf->size);

    if (task == NULL) {
        error = TD_ENOMEM;
        td_emsg(TD_EMSG_NOMEM);
        goto _exit;
    }

    todo_push(todo, task);
    hbuf_free(buf);

    td_try_write(path, todo);
    goto _exit;
}

_exit:
    todo_free(todo);
    hbuf_free(path);
    return error;
}

void
td_emsg(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

void
td_exit(int code)
{
    exit(code);
}

void
td_version(void)
{
    println("todo@%s", TD_VERSION);
}

void
td_help(void)
{
    println("Usage:");
    println("  todo [-h|-v|-a]");
    println("  todo (<id> [done|undo|remove])|<task>..");
    println("");
    println("Examples:");
    println("  add a task       -  todo Go shopping");
    println("  check a task     -  todo 1 done");
    println("  undo a task      -  todo 1 undo");
    println("  remove a task    -  todo 1 rm/remove");
    println("  list undo tasks  -  todo");
    println("  list all tasks   -  todo --all");
    println("  clear done tasks -  todo clean/cleanup");
    println("  clear all tasks  -  todo clear");
    println("");
    println("GitHub: https://github.com/hit9/todo.c <hit9@icloud.com>");
}

void
td_task_print(task_t *task, size_t idx)
{
    hbuf_t *buf = hbuf_new(BUF_UNIT);

    if (buf == NULL) {
        td_emsg(TD_EMSG_NOMEM);
        td_exit(TD_ENOMEM);
    }

    int color = red;
    char *mask = "✖";

    if (task->state == done) {
        color = green;
        mask = "✓";
    }

    hbuf_sprintf(buf, "%lu. \033[%dm%s\033[0m %s\n",
            idx, color, mask, hbuf_str(task->data));
    hbuf_print(buf);
    hbuf_free(buf);
}

hbuf_t *
td_try_txt()
{
    const char *htxt = "~/todo.txt";
    const char *ctxt = "./todo.txt";

    hbuf_t *buf = hbuf_new(BUF_UNIT);

    if (buf == NULL) {
        td_emsg(TD_EMSG_NOMEM);
        td_exit(TD_ENOMEM);
    }

    if (file_exists(ctxt)) {
        if (hbuf_puts(buf, (char *)ctxt) != HBUF_OK) {
            td_emsg(TD_EMSG_NOMEM);
            hbuf_free(buf);
            td_exit(TD_ENOMEM);
        }
    } else {
        wordexp_t exp_r;
        wordexp(htxt, &exp_r, 0);
        char *path = exp_r.we_wordv[0];

        if (!file_exists(path) && !file_touch(path)) {
            td_emsg(TD_EMSG_IOW, path);
            hbuf_free(buf);
            wordfree(&exp_r);
            td_exit(TD_EIOW);
        }

        if (hbuf_puts(buf, path) != HBUF_OK) {
            td_emsg(TD_EMSG_NOMEM);
            wordfree(&exp_r);
            hbuf_free(buf);
            td_exit(TD_ENOMEM);
        }

        wordfree(&exp_r);
    }
    return buf;
}

hbuf_t *
td_try_read(hbuf_t *path)
{
    assert(path != NULL);

    hbuf_t *buf = hbuf_new(FILE_READ_BUF_UNIT);

    if (buf == NULL) {
        td_emsg(TD_EMSG_NOMEM);
        hbuf_free(path);
        td_exit(TD_ENOMEM);
    }

    switch (file_read(buf, (const char *)hbuf_str(path))) {
        case TD_ENOMEM:
            td_emsg(TD_EMSG_NOMEM);
            hbuf_free(buf);
            hbuf_free(path);
            td_exit(TD_ENOMEM);
        case TD_EIOR:
            td_emsg(TD_EMSG_IOR, hbuf_str(path));
            hbuf_free(buf);
            hbuf_free(path);
            td_exit(TD_ENOMEM);
        case TD_OK:
            break;  // defer release `path`
    }

    return buf;
}

todo_t *
td_try_parse(hbuf_t *path)
{
    assert(path != NULL);

    hbuf_t *buf = td_try_read(path);

    parser_result_t *res = todo_parse(buf);

    if (res == NULL) {
        td_emsg(TD_EMSG_NOMEM);
        td_exit(TD_ENOMEM);
    }

    todo_t *todo = NULL;

    switch(res->error) {
        case TD_ENOMEM:
            td_emsg(TD_EMSG_NOMEM);
            hbuf_free(buf);
            parser_result_free(res);
            hbuf_free(path);
            td_exit(TD_ENOMEM);
        case TD_ESYNTAX:
            td_emsg(TD_EMSG_SYNTAX, res->lineno);
            hbuf_free(buf);
            parser_result_free(res);
            hbuf_free(path);
            td_exit(TD_ENOMEM);
        case TD_OK:  // defer relase `path`
            todo = res->todo;
            hbuf_free(buf);
            parser_result_free(res);
    }
    return todo;
}

void
td_try_write(hbuf_t *path, todo_t *todo)
{
    assert(path != NULL);
    assert(todo != NULL);

    hbuf_t *buf = todo_generate(todo);

    if (buf == NULL) {
        td_emsg(TD_EMSG_NOMEM);
        hbuf_free(buf);
        todo_free(todo);
        hbuf_free(path);
        td_exit(TD_ENOMEM);
    }

    switch (file_write((const char *)hbuf_str(path), buf)) {
        case TD_ENOMEM:
            td_emsg(TD_EMSG_NOMEM);
            hbuf_free(buf);
            todo_free(todo);
            hbuf_free(path);
            td_exit(TD_ENOMEM);
        case TD_EIOW:
            td_emsg(TD_EMSG_IOW, hbuf_str(path));
            hbuf_free(buf);
            todo_free(todo);
            hbuf_free(path);
            td_exit(TD_EIOW);
        case TD_OK:  // defer release `todo` & `path`
            hbuf_free(buf);
            break;
    }
}

void
td_ls_all(todo_t *todo)
{
    assert(todo != NULL);

    task_t *task = todo->head;
    size_t idx = 1;

    while (task != NULL) {
        td_task_print(task, idx);
        task = task->next;
        idx += 1;
    }
}

void
td_ls_undo(todo_t *todo)
{
    assert(todo != NULL);

    task_t *task = todo->head;
    size_t idx = 1;

    while (task != NULL) {
        if (task->state == undo)
            td_task_print(task, idx);
        task = task->next;
        idx += 1;
    }
}
