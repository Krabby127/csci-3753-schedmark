CFLAGS=-Wall -Wextra -std=gnu99
CC=gcc
BINS=prime schedule rw


.PHONY: all
all: $(BINS)

prime: prime.c primes.o
	$(CC) $(CFLAGS) -o $@ $^

schedule: schedule.c
	$(CC) $(CFLAGS) -o $@ $^

rw: rw.c
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

.PHONY: benchmark
benchmark:
	./benchmark 20

.PHONY: plot
plot:
	./analyze
	./plot

.PHONY: report
report: report.pdf

report.pdf: report.md
	pandoc --filter ./pandoc-include report.md -o report.pdf

.PHONY: clean-results
clean-results:
	-rm -R results
	-rm -R plots

.PHONY: clean
clean:
	-rm *.o *.out*
	-rm $(BINS)
	-rm -R tmp
	-rm report.pdf
