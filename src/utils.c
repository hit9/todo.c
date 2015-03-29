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

#include "utils.h"

/**
 * Test if file exists.
 */
int
file_exists(const char *path)
{
    return access(path, F_OK) != -1;
}

/**
 * Touch a file.
 */
int
file_touch(const char *path)
{
    FILE *fp = fopen(path, "w");
    int res = (fp != NULL);
    fclose(fp);
    return res;
}

/**
 * Read file into buffer.
 */
int
file_read(hbuf_t *buf, const char *path)
{
    assert(buf != NULL && path != NULL);
    assert(buf->size <= buf->cap);

    FILE *fp = fopen(path, "r");

    if (fp == NULL)
        return TD_EIOR;

    int bytes;

    while (1) {
        if (hbuf_grow(buf, buf->size + FILE_READ_BUF_UNIT) != HBUF_OK)
            return TD_ENOMEM;

        bytes = fread(buf->data + buf->size, sizeof(uint8_t),
                buf->cap - buf->size, fp);

        if (bytes <= 0)
            break;

        buf->size += bytes;
    }

    fclose(fp);
    return TD_OK;
}


/**
 * Write buffter to file.
 */
int
file_write(const char *path, hbuf_t *buf)
{
    assert(buf != NULL);

    FILE *fp = fopen(path, "w");

    if (fp == NULL)
        return TD_EIOW;

    if (fwrite(buf->data, sizeof(uint8_t), buf->size, fp) != buf->size) {
        fclose(fp);
        return TD_EIOW;
    }

    fclose(fp);
    return TD_OK;
}

void
println(const char *str)
{
    printf("%s\n", str);
}
