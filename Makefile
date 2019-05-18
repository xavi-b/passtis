CC=gcc
CFLAGS=-Wall -O
LDFLAGS=
EXEC=mon_executable
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

all: $(EXEC)

pastees:    $(OBJ)
            $(CC) -o $@ $^ $(LDFLAGS)

main.o: pastees.h

%.o:        %.c
            $(CC) -o $@ -c $< $(CFLAGS)

clean:
    rm -f *.o core

mrproper: clean
    rm -f $(EXEC)
