/*
 * Todo's generator
 */

#ifndef GENERATOR_H
#define GENERATOR_H

#include <stddef.h>
#include <stdint.h>
#include "todo.h"
#include "buffer.h"

/* api */

/* generate string from todo struct */
void todo_generate(todo_t *, buf_t *);

#endif
