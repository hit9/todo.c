todo.c
------

Command line lightweight todo tool with readable storage , written in C

![](screen-shot.png)

Installation
------------

    sudo make install

This command will install `todo` to `/usr/bin/todo`.

Usage
-----

`todo` will always try to find `todo.txt` in current directory.

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

Storage
-------

`todo` uses `./todo.txt` for persistent storage.

If you are using `todo` in a "fresh" directory, you need to `touch todo.txt` there.

The storage format is readable, it's the *GitHub Flavored Markdown Task list* :

```markdown
- [x] Go shopping
- [ ] Clean the room
```

FAQ
---

- Why C language? - I just want to write some C code that time.
- Why not `~/todo.txt`? - I need different todos for different projects.
- What dose this tool created for? - I can manage tasks in my terminal, and read it anywhere(for instance, on github.com).

License
--------

BSD
