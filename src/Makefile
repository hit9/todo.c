NAME = todo

INC = ./

SRC =	main.c \
	hbuf.c \
	generator.c \
	parser.c \
	todo.c \
	utils.c

CC = gcc

OBJ = $(SRC:c=o)

UNAME := $(shell uname)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) -I$(INC)

all: $(NAME)

clean:
	rm -rf *.o todo

test: $(NAME)
	touch todo.txt
ifeq ($(UNAME), Linux)
	touch $(NAME).log
	@printf "test add task (1) 	=> "
	@env MALLOC_TRACE=$(NAME).log ./$(NAME) task1 && mtrace $(NAME) $(NAME).log
	@printf "test add task (2) 	=> "
	@env MALLOC_TRACE=$(NAME).log ./$(NAME) task2 && mtrace $(NAME) $(NAME).log
	@printf "test add task (3) 	=> "
	@env MALLOC_TRACE=$(NAME).log ./$(NAME) task3 && mtrace $(NAME) $(NAME).log
	@printf "test task done (1)	=> "
	@env MALLOC_TRACE=$(NAME).log ./$(NAME) 1 done && mtrace $(NAME) $(NAME).log
	@printf "test task done (2)	=> "
	@env MALLOC_TRACE=$(NAME).log ./$(NAME) 2 done && mtrace $(NAME) $(NAME).log
	@printf "test task undo (1)	=> "
	@env MALLOC_TRACE=$(NAME).log ./$(NAME) 2 undo && mtrace $(NAME) $(NAME).log
	@printf "test task rm (1)	=> "
	@env MALLOC_TRACE=$(NAME).log ./$(NAME) 2 rm && mtrace $(NAME) $(NAME).log
	@printf "test task get (1)	=> "
	@env MALLOC_TRACE=$(NAME).log ./$(NAME) 1 && mtrace $(NAME) $(NAME).log
	@printf "test ls undo tasks (1)	=> \n"
	@env MALLOC_TRACE=$(NAME).log ./$(NAME) && mtrace $(NAME) $(NAME).log
	@printf "test ls all tasks (1)	=> \n"
	@env MALLOC_TRACE=$(NAME).log ./$(NAME) --all && mtrace $(NAME) $(NAME).log
	@printf "test task clean (1)	=> "
	@env MALLOC_TRACE=$(NAME).log ./$(NAME) clean && mtrace $(NAME) $(NAME).log
	@printf "test task clear (1)	=> "
	@env MALLOC_TRACE=$(NAME).log ./$(NAME) clear && mtrace $(NAME) $(NAME).log
	rm $(NAME).log
else
	./$(NAME) task1
	./$(NAME) task2
	./$(NAME) task3
	./$(NAME) 1 done
	./$(NAME) 2 done
	./$(NAME) 2 undo
	./$(NAME) 2 rm
	./$(NAME) 1
	./$(NAME) --all
	./$(NAME)
	./$(NAME) clean
	./$(NAME) clear
endif
	rm todo.txt
