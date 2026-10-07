#ifndef _CHACHA_INCLUDED
#define _CHACHA_INCLUDED
#include <stdint.h>

/*
	ChaCha has 20 rounds usually and that's what I recommend, but it's theorized that with 8 rounds is secure.

	https://en.wikipedia.org/wiki/Salsa20#Reduced-round_ChaCha

	20 has 10 column and 10 diagonal rounds.
	12 has 6  column and 6  diagonal rounds.
	8  has 4  column and 4  diagonal rounds.

	These constants may be kinda useless, but I think it clears things up a little.
*/

const uint8_t CHACHA20_ROUNDS = 20;
const uint8_t CHACHA12_ROUNDS = 12;
const uint8_t CHACHA8_ROUNDS  = 8;

/*
	Xor buf with a keystream generated from ChaCha.

	Notes:
	
	The number of rounds should be divisible by 2. (20,12,8)
*/
extern uint32_t chacha_xor(uint8_t* key, uint8_t* nonce, uint8_t* buf, uint64_t bufsize, uint32_t ctr, int rounds);

/*
	Runs the ChaCha block by itself.

	Notes:
	
	The number of rounds should be divisible by 2. (20,12,8)
	The block parameter must be >= 64 bytes in size
*/
extern void chacha_block(uint8_t* key, uint8_t* nonce, uint32_t ctr, uint8_t* block, int rounds);
#endif