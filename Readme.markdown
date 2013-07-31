todo.c
------

Command line lightweight todo tool with readable storage , written in C

latest version: v0.2.2

![](screen-shot.png)

Installation
------------

    sudo make install

This command will install `todo` to `/usr/bin/todo`.

Usage
-----

*Note*: todo will **always** use `./todo.txt` prior to `~/todo.txt` for persistent storage.

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
  clear done tasks -  todo cleanup
  clear all tasks  -  todo clear
```

Storage
-------

todo will always use `./todo.txt` prior to `~/todo.txt` for persistent storage, if there is a `todo.txt` in 
current directory, `todo` will use it, else `todo` will try to find `~/todo.txt`, if it dosn't exist, `todo`
touch an empty file `~/todo.txt`.

So when your are not in home directory, to new a todo, just:

    $ touch todo.txt

The storage format is readable, it's the [GitHub Flavored Markdown Task list](https://help.github.com/articles/github-flavored-markdown#task-lists):

```markdown
- [x] Go shopping
- [ ] Clean the room
```

FAQ
---

- Why C language? - I just want to write some C code that time.
- Why not only `~/todo.txt`? - I need different todos for different projects.
- What dose this tool created for? - I can manage tasks in my terminal, and read it anywhere(for instance, on github.com).

Thanks
------

- Thanks to sjl, todo is inspired by [sjl/t](http://github.com/sjl/t).

- Thanks to vesln's [todo](https://github.com/vesln/todo).

License
--------

BSD
