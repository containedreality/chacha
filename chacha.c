#include <immintrin.h>
#include <string.h>
#include <stdint.h>

#define ROTL32(x, n) ((x << n) | (x >> (32 - n)))

#define CHACHA_QR(a, b, c, d) { a += b; d ^= a; d = ROTL32(d,16);\
	c += d; b ^= c; b = ROTL32(b, 12);\
	a += b; d ^= a; d = ROTL32(d, 8);\
	c += d; b ^= c; b = ROTL32(b, 7); }
	
void chacha_block(uint8_t* key, uint8_t* nonce, uint32_t ctr, uint8_t* block, int rounds)
{
	uint32_t state[16];
	
	state[0] = 0x61707865;
	state[1] = 0x3320646e;
	state[2] = 0x79622d32;
	state[3] = 0x6b206574;
	
	#if defined(__AVX2__)
	_mm256_storeu_si256(
		(__m256i*)&state[4],
		_mm256_loadu_si256((const __m256i*)key)
	);
	#else
	memcpy(&state[4], key, 8 * sizeof(uint32_t));
	#endif
	
	state[12] = ctr;

	memcpy(&state[13], nonce, 3 * sizeof(uint32_t));

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

	#if defined(__AVX2__)
	__m256i s1 = _mm256_loadu_si256((__m256i*)&state[0]);
	__m256i i1 = _mm256_loadu_si256((__m256i*)&initial_state[0]);
	_mm256_storeu_si256((__m256i*)&state[0], _mm256_add_epi32(s1, i1));

	__m256i s2 = _mm256_loadu_si256((__m256i*)&state[8]);
	__m256i i2 = _mm256_loadu_si256((__m256i*)&initial_state[8]);
	_mm256_storeu_si256((__m256i*)&state[8], _mm256_add_epi32(s2, i2));
	#else
	for(int i = 0; i < 16; i++) {
		state[i] += initial_state[i];
	}
	#endif

	#if defined(__AVX2__)
	__m256i row1 = _mm256_loadu_si256((const __m256i*)state);
    __m256i row2 = _mm256_loadu_si256((const __m256i*)(state + 8));

    _mm256_storeu_si256((__m256i*)block, row1);
    _mm256_storeu_si256((__m256i*)(block + 8), row2);
	#else
	memcpy(block, state, 64);
	#endif
}

uint32_t chacha_xor(uint8_t* key, uint8_t* nonce, uint8_t* buf, uint64_t bufsize, uint32_t ctr, int rounds)
{
	uint8_t block[64];
	uint64_t xorred = 0;
	uint64_t remaining = 0;
	
	for(;;) {
		chacha_block(key, nonce, ctr, block, rounds);

		#if defined(__AVX2__)
		remaining = bufsize - xorred;

		if (remaining >= 64) {
			__m256i vbuf0 = _mm256_loadu_si256((__m256i*)(buf + xorred));
			__m256i vbuf1 = _mm256_loadu_si256((__m256i*)(buf + xorred + 32));
			
			__m256i vblk0 = _mm256_loadu_si256((__m256i*)(block));
			__m256i vblk1 = _mm256_loadu_si256((__m256i*)(block + 32));
			
			_mm256_storeu_si256((__m256i*)(buf + xorred),      _mm256_xor_si256(vbuf0, vblk0));
			_mm256_storeu_si256((__m256i*)(buf + xorred + 32), _mm256_xor_si256(vbuf1, vblk1));
			
			xorred += 64;
		} else {
			for(int i = 0; i < remaining; i++) {
				buf[xorred + i] ^= block[i];
					
				return ctr;
			}
		}
		#else
		for(int i = 0; i < 64; i++) {
			if (xorred == bufsize) {
				return ctr;
			}
			
			buf[xorred] ^= block[i];
			xorred++;
		}
		#endif

		ctr++;
	}

	return ctr;
}