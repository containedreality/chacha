#include <stdio.h>
#include <stdint.h>

#include "chacha.h"

int main(void)
{
	puts("https://www.rfc-editor.org/info/rfc8439/#section-2.4.2");
	
	uint8_t key[32] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};
	uint8_t nonce[12] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x4a,0x00,0x00,0x00,0x00};
	
	uint8_t buf[114] = "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it.";
	
	// encrypt
	uint32_t n = chacha_xor(key, nonce, buf, 114, 1, CHACHA20_ROUNDS);
	printf("%u\n", n);
	
	for(int i = 0; i < 114; i++) {
		if ((i%16)== 0) {
			putchar(0xa);
		}
		
		printf("%02x ", buf[i]);
	}
	putchar(0xa);
	
	// decrypt
	chacha_xor(key, nonce, buf, 114, 1, CHACHA20_ROUNDS);
	
	for(int i = 0; i < 114; i++) {
		if ((i%16)== 0) {
			putchar(0xa);
		}
		
		printf("%02x ", buf[i]);
	}
	putchar(0xa);
	
	return 0;
}