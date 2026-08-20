CC = gcc
CFLAGS = -O2 -Wall -std=c17 -fsanitize=address,undefined

all: chacha.o test.o

test: test.o
	./test.o

chacha.o: chacha.c
	$(CC) $(CFLAGS) -c $^ -o $@

test.o: test.c chacha.o
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o