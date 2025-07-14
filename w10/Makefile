# Makefile to compile all .c files individually
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -lm
SRC := $(wildcard *.c)
EXE := $(SRC:.c=)

# .PHONY: all

all: $(EXE)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(EXE)
