NAME = todo

INC = ./

SRC =	main.c \
	buffer.c \
	generator.c \
	parser.c \
	todo.c \
	utils.c

CC = gcc

OBJ = $(SRC:c=o)

CFLAGS = -W -Wall -Wextra

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME) -I$(INC)

all: $(NAME)

clean:
	rm -rf *.o todo

