#include <string.h>
#include <stdint.h>

#define ROTL32(x, n) ((x << n) | (x >> (32 - n)))

#define CHACHA_QR(a, b, c, d) { a += b; d ^= a; d = ROTL32(d,16);\
	c += d; b ^= c; b = ROTL32(b, 12);\
	a += b; d ^= a; d = ROTL32(d, 8);\
	c += d; b ^= c; b = ROTL32(b, 7); }
	
void chacha_block(uint8_t* key, uint8_t* nonce, uint32_t ctr, uint8_t* block, int rounds)
{
	uint32_t* key32 = (uint32_t*)key;
	uint32_t* nonce32 = (uint32_t*)nonce;
	uint32_t state[16];
	
	state[0] = 0x61707865;
	state[1] = 0x3320646e;
	state[2] = 0x79622d32;
	state[3] = 0x6b206574;
	
	for(int i = 0; i < 8; i++) {
		state[4+i] = key32[i];
	}
	
	state[12] = ctr;
	
	for(int i = 0; i < 3; i++) {
		state[13+i] = nonce32[i];
	}
	
	uint32_t initial_state[16];
	memcpy(initial_state, state, 64);
	
	for(int i = 0; i < rounds / 2; i++) {
		// odd round
		CHACHA_QR(state[0], state[4], state[8] , state[12]);
		CHACHA_QR(state[1], state[5], state[9] , state[13]);
		CHACHA_QR(state[2], state[6], state[10], state[14]);
		CHACHA_QR(state[3], state[7], state[11], state[15]);

		// even round
		CHACHA_QR(state[0], state[5], state[10], state[15]);
		CHACHA_QR(state[1], state[6], state[11], state[12]);
		CHACHA_QR(state[2], state[7], state[8] , state[13]);
		CHACHA_QR(state[3], state[4], state[9] , state[14]);
	}
	
	for(int i = 0; i < 16; i++) {
		state[i] += initial_state[i];
	}
	
	memcpy(block, state, 64);
}

uint32_t chacha_xor(uint8_t* key, uint8_t* nonce, uint8_t* buf, uint64_t bufsize, uint32_t ctr, int rounds)
{
	uint8_t block[64];
	uint64_t xorred = 0;
	
	for(;;) {
		chacha_block(key, nonce, ctr, block, rounds);
		
		for(int i = 0; i < 64; i++) {
			if (xorred == bufsize) {
				return ctr;
			}
			
			buf[xorred] ^= block[i];
			
			xorred++;
		}
		
		ctr++;
	}

	return ctr;
}