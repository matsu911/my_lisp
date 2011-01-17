CC = gcc
SRCS  = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))
CFLAGS = -std=c99 -Wall -g -O0
TEST_OBJS = $(filter-out main.o,$(OBJS))

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

all: $(OBJS)
	$(CC) -o lisp $^

unittest_main: $(TEST_OBJS)
	$(CC) -o $@ $^

check: unittest_main
	./unittest_main

clean:
	rm -f *.o

rebuild: clean all