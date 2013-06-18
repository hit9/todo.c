#include "generator.h"
#include <stdio.h>

/*
 * generate todo `td` to buffer `buf`
 * for each task, performs like this:
 *   - [ ] 'xxxx'
 */
void
todo_generate(todo_t *td, buf_t *buf)
{
    task_t *t;
    size_t b_size = 7 * sizeof(uint8_t), // the minimal length of task's string format. "- [ ] \n"
           sz;  // record each task's size
    uint8_t *ptr=0;  // current data pointer

    for (t=td->head; t; t=t->next) {
        /* caculate the required size */
        sz = b_size + t->c_size ;  // string format task's length

        // grow more `sz` size memory to append the new task
        buf_grow(buf, buf->size + sz);
        ptr = buf->data + buf->size;  // get current data tail pointer

        sprintf(ptr, "- [%c] %.*s\n", t->state == done?'x':' ', t->c_size, t->content);
        buf->size += sz;  // increase the buf's size record
    }
}
