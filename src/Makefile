CC = cc
CFLAGS = -Wall

# Executable file
TARGET = todo

# Source and object files
SRC = $(wildcard ./*.c)
OBJ = $(SRC:c=o)


$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

all: $(TARGET)

clean:
	@echo cleaning
	@rm -rf *.o todo

.PHONY: clean
