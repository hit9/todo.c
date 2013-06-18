todo.c
------

Lightweight todo tool in linux terminal, written in C.

![](screen-shot.png)

Installation
------------

    sudo make install


Usage
-----

You need to touch a file named `todo.txt` to let `todo` work.

`todo` will always try to find `./todo.txt`.

```
Usage:
  todo [-h|-v|-a]
  todo (<id> [done|undo|remove])|<task>...
Examples:
  add a task       -  todo Go shopping
  check a task     -  todo 1 done
  undo a task      -  todo 1 undo
  remove a task    -  todo 1 remove
  list undo tasks  -  todo
  list all tasks   -  todo --all
  clear all tasks  -  todo clear
```


License
--------

BSD
