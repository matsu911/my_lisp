CC = gcc
SRCS  = $(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))

%.o: %.c
	$(CC) $(CCFLAGS) -o $@ -c $<

all: $(OBJS)
	$(CC) -o lisp $?

unittest_main: $(filter-out main.o,$(OBJS))
	$(CC) -o $@ $?

check: unittest_main
	./unittest_main

clean:
	rm -f *.o

rebuild: clean all