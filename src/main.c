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

const char *try_txt();
void show_help();
void print_task(task_t *, size_t);


int main(int argc, const char *argv[])
{
    const char *path = try_txt();

    hbuf_t *buf = hbuf_new(FILE_READ_BUF_UNIT);

    if (file_read(buf, path) == TD_OK) {
    }

    parser_result_t *res = todo_parse(buf);

    if (res->error == TD_OK) {
        task_t *task = res->todo->head;
        size_t idx = 1;

        while (task != NULL) {
            print_task(task, idx);
            task = task->next;
            idx += 1;
        }
    }

    free(buf);
    show_help();
    return 0;
}

// need free: wordfree(&exp_r)
const char *try_txt()
{
    const char *home_txt = "~/todo.txt";
    const char *curr_txt = "./todo.txt";

    // find abspath for home_txt
    wordexp_t exp_r;
    wordexp(home_txt, &exp_r, 0);
    const char *home_txt_ = exp_r.we_wordv[0];

    if (file_exists(curr_txt))
        return curr_txt;

    if (file_exists(home_txt_))
        return home_txt_;

    if (file_touch(home_txt_))
        return home_txt_;

    return NULL;  // EIO
}

void
show_help()
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
print_task(task_t *task, size_t idx)
{
    hbuf_t *buf = hbuf_new(BUF_UNIT);

    if (buf == NULL)
        return;

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
