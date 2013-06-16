all:
	gcc buffer.c main.c todo.c parser.c -o test
	./test
