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

test:
	make -C src/ test
