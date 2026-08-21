/*
crude benchmarking program
*/
#include <stdio.h>
#include <stdint.h>
#include <time.h> 

#include "chacha.h"

int main(void)
{
	#if defined(__AVX2__)
	printf("avx2: yes\n");
	#else
	printf("avx2: no\n");
	#endif
	uint8_t key[32] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};
	uint8_t nonce[12] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x4a,0x00,0x00,0x00,0x00};
	
	uint8_t buf[114] = "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it.";
	
	uint8_t block[64];

	time_t ctime = time(NULL);

	uint64_t blocks = 0;

	while((time(NULL)) < (ctime + 5)) {
		chacha_block(key, nonce, blocks, block, CHACHA20_ROUNDS);
		blocks++;
	}

	printf("chacha20 blocks generated within 5 seconds: %lu\n", blocks);

	ctime = time(NULL);
	blocks = 0;
	while((time(NULL)) < (ctime + 5)) {
		chacha_block(key, nonce, blocks, block, CHACHA12_ROUNDS);
		blocks++;
	}
	printf("chacha12 blocks generated within 5 seconds: %lu\n", blocks);

	ctime = time(NULL);
	blocks = 0;
	while((time(NULL)) < (ctime + 5)) {
		chacha_block(key, nonce, blocks, block, CHACHA8_ROUNDS);
		blocks++;
	}
	printf("chacha8  blocks generated within 5 seconds: %lu\n", blocks);
	
	printf("testing Xor on the test\n");

	ctime = time(NULL);
	uint64_t bytes = 0;
	int ctr = 0;
	while((time(NULL)) < (ctime + 5)) {
		chacha_xor(key, nonce, buf, 114, ctr, CHACHA20_ROUNDS);
		ctr++;
		bytes += 114;
	}

	printf("chacha20 xor bytes/5s: %lu\n", bytes);

	ctime = time(NULL);
	bytes = 0;
	ctr = 0;
	while((time(NULL)) < (ctime + 5)) {
		chacha_xor(key, nonce, buf, 114, ctr, CHACHA12_ROUNDS);
		ctr++;
		bytes += 114;
	}

	printf("chacha12 xor bytes/5s: %lu\n", bytes);

	ctime = time(NULL);
	bytes = 0;
	ctr = 0;
	while((time(NULL)) < (ctime + 5)) {
		chacha_xor(key, nonce, buf, 114, ctr, CHACHA8_ROUNDS);
		ctr++;
		bytes += 114;
	}

	printf("chacha8  xor bytes/5s: %lu\n", bytes);

	return 0;
}