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


#ifndef __HBUF_H
#define __HBUF_H

#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#define HBUF_MAX_SIZE 16 * 1024 * 1024  //16mb

typedef enum {
    HBUF_OK = 0,
    HBUF_ENOMEM = 1,
    HBUF_EFAILED = 2,
} hbuf_error_t;

typedef struct hbuf_st {
    uint8_t *data;      /* real data */
    size_t size;        /* real data size */
    size_t cap;         /* buf cap */
    size_t unit;        /* reallocation unit size */
} hbuf_t;


hbuf_t *hbuf_new(size_t);
void hbuf_free(hbuf_t *);
void hbuf_clear(hbuf_t *);
int hbuf_grow(hbuf_t *, size_t);
char *hbuf_str(hbuf_t *);
void hbuf_print(hbuf_t *);
void hbuf_println(hbuf_t *);
int hbuf_put(hbuf_t *, uint8_t *, size_t);
int hbuf_putc(hbuf_t *, char);
int hbuf_puts(hbuf_t *, char *);
void hbuf_lrm(hbuf_t *, size_t);
void hbuf_rrm(hbuf_t *, size_t);
int hbuf_sprintf(hbuf_t *, const char *, ...);

#ifdef __cplusplus
}
#endif

#endif
