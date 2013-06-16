#include "buffer.h"
#include <stdlib.h>
#include <assert.h>

#define BUFFER_MAX_ALLOCATE_SIZE (16 * 1024 * 1024) // 16M


/* initialize buf , return the pointer */
buf_t *
buf_new(size_t unit)
{
    buf_t *buf = (buf_t *)malloc(sizeof(buf_t));

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
    assert(buf && buf->unit);  /* assuming buf not NULL and writable */

    if (target_size > BUFFER_MAX_ALLOCATE_SIZE) /* over max buffer allocate memory size */
        return NO_MEM;

    if (buf->a_size >= target_size)
        return OK;

    size_t sz = 0;

    while (sz < target_size)
        sz += buf->unit;  /* increase sz by unit size each time, until > target_size */

    uint8_t *new_data = (uint8_t *)realloc(buf->data, sz);

    if (!new_data)
        return NO_MEM;

    buf->data = new_data;
    buf->a_size = sz;
    return OK;
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
