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

#include "hbuf.h"

/**
 * New buf.
 */
hbuf_t *
hbuf_new(size_t unit)
{
    hbuf_t *buf = malloc(sizeof(hbuf_t));

    if (buf != NULL) {
        buf->data = NULL;
        buf->size = 0;
        buf->cap = 0;
        buf->unit = unit;
    }

    return buf;
}

/**
 * Free buf.
 */
void
hbuf_free(hbuf_t *buf)
{
    if (buf != NULL) {
        if (buf->data != NULL)
            free(buf->data);
        free(buf);
    }
}


/**
 * Free buf data.
 */
void
hbuf_clear(hbuf_t *buf)
{
    assert(buf != NULL);

    if (buf->data != NULL)
        free(buf->data);
    buf->data = NULL;
    buf->size = 0;
    buf->cap = 0;
}

/**
 * Increase buf allocated size to `size`, O(1), O(n)
 */
int
hbuf_grow(hbuf_t *buf, size_t size)
{
    assert(buf != NULL && buf->unit != 0);

    if (size > HBUF_MAX_SIZE)
        return HBUF_ENOMEM;

    if (size <= buf->cap)
        return HBUF_OK;

    size_t cap = buf->cap + buf->unit;

    while (cap < size)
        cap += buf->unit;

    uint8_t *data = realloc(buf->data, cap);

    if (data == NULL)
        return HBUF_ENOMEM;

    buf->data = data;
    buf->cap = cap;
    return HBUF_OK;
}

/**
 * Get data as c string (terminate with '\0'), O(1), O(n)
 */
char *
hbuf_str(hbuf_t *buf)
{
    assert(buf && buf->unit);

    if (buf->size < buf->cap && buf->data[buf->size] == '\0')
        return (char *)buf->data;

    if (buf->size + 1 <= buf->cap ||
            hbuf_grow(buf, buf->size + 1) == HBUF_OK) {
        buf->data[buf->size] = '\0';
        return (char *)buf->data;
    }

    return NULL;
}

/**
 * Put a char to buf, O(1), O(n)
 */
int
hbuf_putc(hbuf_t *buf, char ch)
{
    int res = hbuf_grow(buf, buf->size + 1);

    if (res != HBUF_OK)
        return res;

    buf->data[buf->size] = ch;
    buf->size += 1;
    return HBUF_OK;
}

/**
 * Print buf to stdout
 */
void
hbuf_print(hbuf_t *buf)
{
    printf("%.*s\n", (int)buf->size, buf->data);
}

/**
 * Put data to buf, O(n)
 */
int
hbuf_put(hbuf_t *buf, uint8_t *data, size_t size)
{
    int result = hbuf_grow(buf, buf->size + size);

    if (result == HBUF_OK) {
        memcpy(buf->data + buf->size, data, size);
        buf->size += size;
    }

    return result;
}

/**
 * Put string to buf, O(n)
 */
int
hbuf_puts(hbuf_t *buf, char *str)
{
    return hbuf_put(buf, (uint8_t *)str, strlen(str));
}


/**
 * Remove left data from buf by number of bytes, O(n)
 */
void
hbuf_lrm(hbuf_t *buf, size_t size)
{
    assert(buf != NULL && buf->unit != 0);

    if (size > buf->size) {
        buf->size = 0;
        return;
    }

    buf->size -= size;
    memmove(buf->data, buf->data + size, buf->size);
}


/**
 * Remove right data from buf by number of bytes, O(1)
 */
void
hbuf_rrm(hbuf_t *buf, size_t size)
{
    assert(buf != NULL && buf->unit != 0);

    if (size > buf->size) {
        buf->size = 0;
        return;
    }
    buf->size -= size;
}
