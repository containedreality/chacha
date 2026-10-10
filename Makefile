CC = gcc
CFLAGS = -O2 -Wall -std=c17 -fsanitize=address,undefined 

CC_BE = mips-linux-gnu-gcc-14
CFLAGS_BE = -O2 -Wall -std=c17 -static
QEMU=qemu-mips

all: chacha.o test.o benchmark.o

test: test.o benchmark.o
	./test.o

chacha.o: chacha.c
	$(CC) $(CFLAGS) -c $^ -o $@

benchmark.o: benchmark.c chacha.o
	$(CC) $(CFLAGS) $^ -o $@

test.o: test.c chacha.o
	$(CC) $(CFLAGS) $^ -o $@

be_chacha.o: chacha.c
	$(CC_BE) $(CFLAGS_BE) -c $^ -o $@

be_test.o: test.c be_chacha.o
	$(CC_BE) $(CFLAGS_BE) $^ -o $@

betest: be_test.o
	$(QEMU) $^

clean:
	rm -f *.o qemu*
