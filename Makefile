CC = gcc
CFLAGS = -O2 -Wall -std=c17 -fsanitize=address,undefined -mavx2

all: chacha.o test.o benchmark.o

test: test.o benchmark.o
	./test.o
	./benchmark.o

chacha.o: chacha.c
	$(CC) $(CFLAGS) -c $^ -o $@

test.o: test.c chacha.o
	$(CC) $(CFLAGS) $^ -o $@

benchmark.o: benchmark.c chacha.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o