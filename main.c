#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

#include "rsa.h"

int main(void) {
	rsa_structure ctx;
	// todo random prime numbers are for sure better idea
	rsa_init(59, 67, &ctx);

	char message[] = "Hello World!";
	uint32_t encrypted_message[sizeof(message)];
	char decrypted_message[sizeof(message)];

	for (uint32_t i = 0; i < sizeof(message); i++) {
		encrypted_message[i] = rsa_encrypt(message[i], rsa_get_public_key(&ctx));
		printf("Char %c encrypted to %d\n", message[i], encrypted_message[i]);
	}

	for (uint32_t i = 0; i < sizeof(message); i++) {
		decrypted_message[i] = rsa_decrypt(encrypted_message[i], &ctx);
	}


	printf("Original message: %s\n", message);
	printf("Decrypted message: %s\n", decrypted_message);
}

