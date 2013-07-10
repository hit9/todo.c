compile:
	make -C src/

clean:
	make -C src/ clean

install: compile
	mv src/todo /usr/bin/
