/*
 * util functions
 */


#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* wrap string color */
uint8_t *
colored(uint8_t *str, size_t size, int color)
{
    if (color < 30 || color > 37)
        color = 37;

    const uint8_t *prefix = "\033[";
    const uint8_t *suffix = "\033[0m";

    size_t sz = strlen(prefix) + 2 + 1 + size + strlen(suffix);

    uint8_t *ret = (uint8_t *)malloc(sz * sizeof(uint8_t));

    sprintf(ret, "%s%dm%.*s%s", prefix, color, size, str, suffix);
    return ret;
}
