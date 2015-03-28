CFLAGS=-Wall -Wextra -std=gnu99
CC=gcc
BINS=prime





.PHONY: all
all: $(BINS)

prime: prime.c primes.o
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c %.h debug.h
	$(CC) $(CFLAGS) -c $<


.PHONY: clean
clean:
	-rm *.o *.out*
	-rm $(BINS)
