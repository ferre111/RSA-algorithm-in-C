#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "rsa.h"

/* gcd - Greatest common divisor */
static int32_t rsa_gcd(int32_t a, int32_t b) {
	int32_t aux_var = 0;

	/* if b is greater than swap their values */
	if (b > a) {
		aux_var = a;
		a = b;
		b = aux_var;
	}

	while(1) {
		aux_var = a % b;
		if (!aux_var) {
			return b;
		}
		a = b;
		b = aux_var;
	}
}

static uint32_t modpow(uint32_t base, uint32_t power, uint32_t mod)
{
    uint32_t result = 1;
    for (uint32_t i = 0; i < power; i++)
    {
            result = (result * base) % mod;
    }
    return result;
}
 
void rsa_init(uint32_t p, uint32_t q, rsa_structure* rsa_ctx) {
	// todo here we can check if p and q are prime numbers	
	uint32_t aux_var = 0;
	rsa_ctx->p_prime = p;
	rsa_ctx->q_prime = q;

	rsa_ctx->public_key.n = rsa_ctx->p_prime * rsa_ctx->q_prime;
	rsa_ctx->totient = (rsa_ctx->p_prime - 1) * (rsa_ctx->q_prime - 1);

	/* public key is any number belonging to Z*fi(n) that means public key and fi(n) are coprime (gcd == 1), find public key */
    // todo it would probably be safer if public key would be choose randomly
	rsa_ctx->public_key.e = 2;
	while (1) {
		aux_var = rsa_gcd(rsa_ctx->public_key.e, rsa_ctx->totient);
		if (aux_var == 1) {
			break;
		}
		rsa_ctx->public_key.e++;
	}

	/* d = e^-1(mod totient), find private key */
	for (rsa_ctx->d = 0; rsa_ctx->d < rsa_ctx->totient; rsa_ctx->d++) {
		aux_var = rsa_ctx->d * rsa_ctx->public_key.e % rsa_ctx->totient;
		if (aux_var == 1) {
			break;
		}
	}
	
	printf("RSA parameters:\np_prime: %ld\nq_prime: %ld\nn: %ld\ntotient: %ld\ne: %ld\nd: %ld\n", rsa_ctx->p_prime, rsa_ctx->q_prime,
		rsa_ctx->public_key.n, rsa_ctx->totient, rsa_ctx->public_key.e, rsa_ctx->d);
}

uint32_t rsa_encrypt(uint32_t msg, rsa_public_key* public_key) {
    assert(msg < public_key->n);
	return modpow(msg, public_key->e, public_key->n);
}

uint32_t rsa_decrypt(uint32_t encrypted_msg, rsa_structure* rsa_ctx) {
	return modpow(encrypted_msg, rsa_ctx->d, rsa_ctx->public_key.n);
}

rsa_public_key *rsa_get_public_key(rsa_structure* rsa_ctx) {
    return &rsa_ctx->public_key;
}
