/*
 * Todo's parser
 */

#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>
#include <stdint.h>
#include "todo.h"

/* api */

/* parse string to todo struct. */
unsigned int todo_parse(todo_t *, uint8_t *, size_t);

#endif
