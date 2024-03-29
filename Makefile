CC       := gcc
CFLAGS   := -Wall -Wextra -std=c11 -g -pedantic -I./src -lncurses

SRC_DIR     := src
INCLUDE_DIR := src
CSRC        := $(wildcard $(SRC_DIR)/*.c)
CHDR        := $(wildcard $(INCLUDE_DIR)/*.h)
OBJ         := $(CSRC:.c=.o)

PROG := cj

.PHONY: all clean

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c $(CHDR)
	$(CC) $(CFLAGS) -MP -c $< -o $@

$(OBJ): $(CHDR)

clean:
	rm $(OBJ) $(PROG)
