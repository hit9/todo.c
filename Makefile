INC_DIR=src/
SRC=src/*.c

all:
	cc ${SRC} -o todo -I${INC_DIR}

test:
	cc ${SRC} -o todo -g -I${INC_DIR}
	touch todo.log
	env MALLOC_TRACE=todo.log ./todo
	@echo "memory leaks check result:"
	mtrace todo todo.log
clean:
	rm todo todo.log
