all: test bench

test: list.h test.h arena.h test.c
	gcc -g test.c -o test

bench: list.h bench.h arena.h bench.c
	gcc bench.c -o bench -O2

clean:
	rm test bench
