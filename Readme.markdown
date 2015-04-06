todo.c
------

Command line lightweight todo tool with readable storage, written in C.

latest version: v0.2.5

![](screen-shot.png)

Installation
------------

    make install

This command will install `todo` to `~/.todo`.

Next, add the following line to your shell's configuration(~/.bashrc or ~/.zshrc etc.):

    alias todo=~/.todo

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
  remove a task    -  todo 1 rm/remove
  list undo tasks  -  todo
  list all tasks   -  todo --all
  clear done tasks -  todo clean/cleanup
  clear all tasks  -  todo clear
```

Storage
-------

todo will always use `./todo.txt` prior to `~/todo.txt` for persistent storage, if there is a `todo.txt` in current directory, `todo` will use it, else `todo` will try to find `~/todo.txt`, if it doesn't exist, `todo` touches an empty file `~/todo.txt`.

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
- What is this tool created for? - I can manage tasks in my terminal, and read it anywhere(for instance, on github.com).

Contributors
------------

https://github.com/hit9/todo.c/graphs/contributors

Thanks
------

- Thanks to sjl, todo is inspired by [sjl/t](http://github.com/sjl/t).

- Thanks to vesln's [todo](https://github.com/vesln/todo).

License
--------

BSD
