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

    const uint8_t *prefix = "\033[";
    const uint8_t *suffix = "\033[0m";

    /* the buffer's size */
    size_t sz = strlen(prefix) + 2 + 1 + size + strlen(suffix);

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
