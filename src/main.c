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

void td_emsg(const char *, ...);
void td_exit(int, const char *, ...);
void td_help();
int td_task_print(task_t *);
int td_try_txt(hbuf_t *);
int td_try_read(hbuf_t *);
int td_try_parse(parser_result_t **);

void
td_emsg(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}

int
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

int
td_task_print(task_t *task)
{
    hbuf_t *buf = hbuf_new(BUF_UNIT);

    if (buf == NULL)
        return TD_ENOMEM;

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
    return TD_OK;
}

int
td_try_txt(hbuf_t *buf)
{
    assert(buf != NULL);

    const char *htxt = "~/todo.txt";
    const char *ctxt = "./todo.txt";

    if (file_exists(ctxt)) {
        if (hbuf_puts(buf, ctxt) != HBUF_OK)
            return TD_ENOMEM;
    } else {
        wordexp_t exp_r;
        wordexp(htxt, &exp_r, 0);
        char *path = exp_r.we_wordv[0];

        if (!file_exists(path) && !file_touch(path)) {
            wordfree(&exp_r);
            return TD_EIOW;
        }

        if (hbuf_puts(buf, path) != HBUF_OK) {
            wordfree(&exp_r);
            return TD_ENOMEM;
        }
    }
    return TD_OK;
}

int
td_try_read(hbuf_t *buf)
{
    assert(buf != NULL);

    hbuf_t *path = hbuf_new(BUF_UNIT);

    if (path == NULL)
        return TD_ENOMEM;

    int error = td_try_txt(path);

    switch (error) {
        case TD_OK:
            error = file_read(buf, hbuf_str(path));
            break;
        case TD_ENOMEM:
            td_emsg(TD_EMSG_NOMEM);
            hbuf_free(path);
            td_exit(TD_ENOMEM);
        case TD_EIOW:
            td_emsg(TD_EMSG_IOW, hbuf_str(path));
            hbuf_free(path);
            td_exit(TD_EIOW);
    }

    hbuf_free(path);
    return error;
}

int
td_try_parse(parser_result_t **res)
{
    hbuf_t *buf = hbuf_new(FILE_READ_BUF_UNIT);

    if (buf == NULL)
        return TD_ENOMEM;

    int error = td_try_read(buf);

    switch (error) {
        case TD_OK:
            *res = todo_parse(buf);
            break;
        case TD_ENOMEM:
            td_emsg(TD_EMSG_NOMEM);
            hbuf_free(buf);
            td_exit(TD_ENOMEM);
        case TD_EIOR:
            td_emsg(TD_EMSG_IOR, hbuf_str(buf));
            hbuf_free(buf);
            td_exit(TD_EIOR);
    }

    hbuf_free(buf);
    return error;
}
