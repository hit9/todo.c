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

#define VERSION "0.2.4"

#define TD_EMSG_NOMEM "No memory"
#define TD_EMSG_IOR "Error to read %s"
#define TD_EMSG_IOW "Error to write %s"
#define TD_EMSG_SYNTAX "Syntax error at line %lu"

void td_emsg(const char *, ...);
void td_exit(int, const char *, ...);
void td_help();
void td_task_print(task_t *);
hbuf_t *td_try_txt();
hbuf_t *td_try_read();
todo_t *td_try_parse();

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
td_help()
{
    println("Usage:");
    println("  todo [-h|-v|-a]");
    println("  todo (<id> [done|undo|remove])|<task>..");
    println("Examples:");
    println("  add a task       -  todo Go shopping");
    println("  check a task     -  todo 1 done");
    println("  undo a task      -  todo 1 undo");
    println("  remove a task    -  todo 1 remove");
    println("  list undo tasks  -  todo");
    println("  list all tasks   -  todo --all");
    println("  clear done tasks -  todo cleanup");
    println("  clear all tasks  -  todo clear");
}

void
td_task_print(task_t *task)
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
        if (hbuf_puts(buf, ctxt) != HBUF_OK) {
            td_emsg(TD_EMSG_NOMEM);
            hbuf_free(buf);
            td_exit(TD_ENOMEM);
        }
    } else {
        wordexp_t exp_r;
        wordexp(htxt, &exp_r, 0);
        char *path = exp_r.we_wordv[0];

        if (!file_exists(path) && !file_touch(path)) {
            td_emsg(TD_EMSG_IOW, hbuf_str(path));
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
td_try_read()
{
    hbuf_t *path = td_try_txt();

    hbuf_t *buf = hbuf_new(FILE_READ_BUF_UNIT);

    if (buf == NULL) {
        td_emsg(TD_EMSG_NOMEM);
        hbuf_free(path);
        td_exit(TD_ENOMEM);
    }

    switch (file_read(buf, hbuf_str(path))) {
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
            hbuf_free(path);
            break;
    }

    return buf;
}

todo_t *
td_try_parse()
{
    hbuf_t *buf = td_try_read();
    parser_result_t *res = todo_parse(buf);

    if (res == NULL) {
        td_emsg(TD_EMSG_NOMEM);
        td_exit(TD_ENOMEM);
    }

    switch(res->error) {
        case TD_ENOMEM:
            td_emsg(TD_EMSG_NOMEM);
            hbuf_free(buf);
            hbuf_free(res);
            td_exit(TD_ENOMEM);
        case TD_ESYNTAX:
            td_emsg(TD_ESYNTAX, res->lineno);
            hbuf_free(buf);
            hbuf_free(res);
            td_exit(TD_ENOMEM);
        case TD_OK:
            todo = res->todo;
            hbuf_free(buf);
            hbuf_free(res);
            break;
    }

    return todo;
}
