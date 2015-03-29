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

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) -I$(INC)

all: $(NAME)

clean:
	rm -rf *.o todo
