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


/*
 * utils functions
 */


#include "utils.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* wrap string color, remember to release the return string
 * after write it to stdout
 */
uint8_t *
colored(uint8_t *str, size_t size, int color)
{
    /* white(37) as default */
    if (color < 30 || color > 37)
        color = 37;

    const uint8_t *prefix = "\033[";  // 4 char
    const uint8_t *suffix = "\033[0m";  // 6 char

    /* the buffer's size */
    size_t sz = (4 + 6 + 2 + 1) * sizeof(uint8_t) + size;

    /* allocate enough buffer */
    uint8_t *ret = (uint8_t *)malloc(sz * sizeof(uint8_t));

    /* mix them in */
    sprintf(ret, "%s%dm%.*s%s", prefix, color, size, str, suffix);

    return ret;
}


/* determine if a string is numberic */

int
is_int_like (const char * s)
{
    if (s == NULL || *s == '\0' || isspace(*s))
      return 0;
    char * p;
    strtol (s, &p, 0);
    return *p == '\0';
}

/* join strings in array `arr` with ' ' */
buf_t *
space_join(uint8_t **arr, size_t len, buf_t *buf)
{
    int i, str_len, sz;
    uint8_t *str, *p;

    for (i=0; i < len; i++) {

        str = *(arr + i);
        sz = str_len = strlen(str);

        /* if not the last array, append a empty space " " */
        if (i != len-1) sz += 1;

        buf_grow(buf, buf->size + sz);

        p = buf->data + buf->size;

        strcpy(p, str);

        /* if not the last array, append a empty space " " */
        if (i != len-1) strcpy(p + str_len, " ");

        buf->size += sz;
    }

    return buf;
}
