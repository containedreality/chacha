# ChaCha

Implementation of the ChaCha stream cipher in C17.

## Example

See [test.c](test.c)

## Implementation Notes

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
