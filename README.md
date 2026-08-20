# ChaCha

Implementation of the ChaCha stream cipher in C17.

## Example

```c
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
```

```console
$ make test
gcc -O2 -Wall -std=c17 -fsanitize=address,undefined -c chacha.c -o chacha.o
gcc -O2 -Wall -std=c17 -fsanitize=address,undefined test.c chacha.o -o test.o
./test.o
https://www.rfc-editor.org/info/rfc8439/#section-2.4.2
2

6e 2e 35 9a 25 68 f9 80 41 ba 07 28 dd 0d 69 81 
e9 7e 7a ec 1d 43 60 c2 0a 27 af cc fd 9f ae 0b 
f9 1b 65 c5 52 47 33 ab 8f 59 3d ab cd 62 b3 57 
16 39 d6 24 e6 51 52 ab 8f 53 0c 35 9f 08 61 d8 
07 ca 0d bf 50 0d 6a 61 56 a3 8e 08 8a 22 b6 5e 
52 bc 51 4d 16 cc f8 06 81 8c e9 1a b7 79 37 36 
5a f9 0b bf 74 a3 5b e6 b4 0b 8e ed f2 78 5e 42 
87 4d 

4c 61 64 69 65 73 20 61 6e 64 20 47 65 6e 74 6c 
65 6d 65 6e 20 6f 66 20 74 68 65 20 63 6c 61 73 
73 20 6f 66 20 27 39 39 3a 20 49 66 20 49 20 63 
6f 75 6c 64 20 6f 66 66 65 72 20 79 6f 75 20 6f 
6e 6c 79 20 6f 6e 65 20 74 69 70 20 66 6f 72 20 
74 68 65 20 66 75 74 75 72 65 2c 20 73 75 6e 73 
63 72 65 65 6e 20 77 6f 75 6c 64 20 62 65 20 69 
74 2e 

```

## Implementation Notes

### Endianness

There's no guarantee that it'll be little-endian or have the correct endianness on all platforms.

### Licensing

All code and documentation in this repository I have placed in the public domain. See `LICENSE` for details. Credit is appreciated though not required.

### Disclaimer

You probably should not use this in production and/or security critical environments without thorough testing. This is primarily for educational and insecure purposes (such as a non-cryptographic PRNG).

This implementation isn't exactly optimized. Also while hard to mess up ChaCha (and I don't think I did), it's not worth the risk.

Instead if you plan on using ChaCha20, I recommend using one of the following libraries:

* [libsodium](https://libsodium.org/)
* [OpenSSL](https://openssl.org/)

## Resources

* [RFC 8439: ChaCha20 and Poly1305 for IETF Protocols](https://www.rfc-editor.org/info/rfc8439/)
