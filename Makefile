CC = gcc
SRCS  = $(wildcard *.c)
OBJS = $(patsubst %.c,%.o,$(SRCS))
CFLAGS = -std=c99 -Wall -g -O0
LDFLAGS = -L/usr/local/lib -lgc
TEST_OBJS = $(filter-out main.o,$(OBJS))

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

all: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o lisp $^

unittest_main: $(TEST_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

check: unittest_main
	./unittest_main

clean:
	rm -f *.o

rebuild: clean all