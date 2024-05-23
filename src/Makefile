CC=gcc
CFLAGS= -Wall -Wextra -g
SRC= ./main.c ./parse.c ./builtin.c ./color.c
OBJ= $(SRC:.c=.o)
EXE= ash

all: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXE)

%.o: %.c 
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJ) $(EXE)

