/*
 * util functions
 */


#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* wrap string color */
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

        str = *(arr+i);
        sz = str_len = strlen(str);

        if (i != len-1)  // not the last
            sz += 1;

        buf_grow(buf, buf->size + sz);

        p = buf->data + buf->size;

        strcpy(p, str);

        if (i != len-1)  // not the last
            strcpy(p+str_len, " ");

        buf->size += sz;
    }

    return buf;
}
