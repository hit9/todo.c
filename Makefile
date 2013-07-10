compile:
	make -C src/

test:
	make -C src/
	touch todo.log
	env MALLOC_TRACE=todo.log src/todo
	@echo "memory leaks check result:"
	mtrace src/todo todo.log
clean:
	make -C src/ clean
	rm -rf todo todo.log

install: compile
	mv src/todo /usr/bin/
