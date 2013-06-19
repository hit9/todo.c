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

#include "generator.h"

#include <stdio.h>

/*
 * generate todo `td` to buffer `buf`
 * for each task, its string format performs like this:
 *   - [ ] 'xxxx'
 */
void
todo_generate(todo_t *td, buf_t *buf)
{
    task_t *t;
    size_t b_size = 7 * sizeof(uint8_t), /* the minimal length of task's string format: "- [ ] \n" */
           sz;  /* record each task's size */
    uint8_t *ptr = 0;  /* current data pointer */

    for (t = td->head; t; t = t->next) {
        /* caculate the required size */
        sz = b_size + t->c_size ;

        /* grow more `sz` size memory to append the new task */
        buf_grow(buf, buf->size + sz);
        /* get current data tail pointer */
        ptr = buf->data + buf->size;
        sprintf(ptr, "- [%c] %.*s\n", t->state == done ? 'x' : ' ', t->c_size, t->content);
        buf->size += sz;
    }
}
