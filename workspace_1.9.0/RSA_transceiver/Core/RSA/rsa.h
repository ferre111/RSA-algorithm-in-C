#include <stdint.h>

typedef struct {
    uint32_t n;
	uint32_t e;
} rsa_public_key;

typedef struct {
	/* p and q must be prime numbers! */
	uint32_t p_prime;
	uint32_t q_prime;
	uint32_t totient;
	/* private key */
	uint32_t d;
    rsa_public_key public_key;
} rsa_structure;


void rsa_init(uint32_t p, uint32_t q, rsa_structure* rsa_ctx);
uint32_t rsa_encrypt(uint32_t msg, rsa_public_key *public_key);
uint32_t rsa_decrypt(uint32_t encrypted_msg, rsa_structure* rsa_ctx);
rsa_public_key *rsa_get_public_key(rsa_structure* rsa_ctx);
