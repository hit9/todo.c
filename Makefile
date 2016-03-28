P2M_OPTS =      -s 1 -r "Alpha" -c "github.com/hit9"

compile:
	make -C src/

clean:
	make -C src/ clean

install: compile
	mv src/todo ~/.todo
	@echo "\n=====> Add the following line to your shell configuration <===== \n"
	@echo "  alias todo=~/.todo"

uninstall:
	rm -fr ~/.todo
	@echo "You can now delete this line from your configuration (~/.bashrc or ~/.zshrc etc.)"
	@echo "  alias todo=~/.todo"

todo.1: 	todo.pod
			pod2man $(P2M_OPTS) $< > $@

test:
	make -C src/ test
