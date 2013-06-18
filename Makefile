all:
	gcc buffer.c main.c todo.c parser.c utils.c -o todo
	./todo
