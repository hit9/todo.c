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
#include <stdlib.h>
#include <assert.h>

#define BUFFER_MAX_ALLOCATE_SIZE (16 * 1024 * 1024) // 16M


/* initialize buffer, return the buffer's pointer */
buf_t *
buf_new(size_t unit)
{
    buf_t *buf = malloc(sizeof(buf_t));

    if (buf) {
        buf->size = 0;
        buf->a_size = 0;
        buf->data = 0;
        buf->unit = unit;
    }

    return buf;
}

/* grow buffer up to given size */
int
buf_grow(buf_t *buf, size_t target_size)
{
    /* assuming buf not NULL and writable */
    assert(buf && buf->unit);

    /* over the max buffer allocate memory size */
    if (target_size > BUFFER_MAX_ALLOCATE_SIZE)
        return BUF_NO_MEM;

    if (buf->a_size >= target_size)
        return BUF_OK;

    size_t sz = buf->a_size;

    /* increase sz by unit size each time, until `sz > target_size` */
    while (sz < target_size)
        sz += buf->unit;

    uint8_t *new_data = (uint8_t *)realloc(buf->data, sz);

    if (!new_data)
        return BUF_NO_MEM;

    buf->data = new_data;
    buf->a_size = sz;

    return BUF_OK;
}

/* release buffer's memory */
void
buf_free(buf_t *buf)
{
    if (buf) {
        free(buf->data);
        free(buf);
    }
}
