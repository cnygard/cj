CC       := gcc
CFLAGS   := -Wall -Wextra -std=c11 -pedantic -I./src -lncurses -Wgnu-pointer-arith

SRC_DIR     := src
INCLUDE_DIR := src
CSRC        := $(wildcard $(SRC_DIR)/*.c)
CHDR        := $(wildcard $(INCLUDE_DIR)/*.h)
OBJ         := $(CSRC:.c=.o)

TEST_SRC := $(filter-out $(SRC_DIR)/main.c, $(CSRC))
TEST_OBJ := $(TEST_SRC:.c=.o)

PROG := cj

.PHONY: all test release debug clean

all: clean $(PROG)

release:
	$(MAKE) all CFLAGS="$(CFLAGS) -O3"

debug: clean
	@echo "Defining DEBUG"
	$(MAKE) all CFLAGS="$(CFLAGS) -g -DDEBUG"

$(PROG): $(OBJ)
	$(CC) $(CFLAGS) -o $@  $^

%.o: %.c $(CHDR)
	$(CC) $(CFLAGS) -o $@  -MP -c $<

test: $(TEST_OBJ) test/test.o
	cp test/golden.txt test/test_pt.txt
	cp test/golden.txt test/test_lt.txt
	$(CC) $(CFLAGS) -O3 -o test/test $^ # replace -O3 with -g to debug
	./test/test

clean:
	rm -f $(OBJ) $(PROG) test/test
