/*
 ============================================================================
 Name        : utncrypto2015.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "ecrypt-sync.h"

int main(void) {

	int accion = 0;
	ECRYPT_ctx * ctx = malloc(sizeof(ECRYPT_ctx));
	u8 * iv = malloc(sizeof(u8));
	u8 * input = malloc(sizeof(u8));
	u8 * output = malloc(sizeof(u8));
	u32 msglen;

	*iv = 0;
	*input = (u8) "A";

	msglen = sizeof(*input);

	u8 * test = malloc(sizeof(u8));
	*test = *input;

	// encripto
	ECRYPT_process_packet(accion, ctx, iv, input, output, msglen);


	accion = 1;
	*input = *output;

	// decripto
	ECRYPT_process_packet(accion, ctx, iv, input, output, msglen);

	if (*test == *output){
		printf("Correcto!");
	} else {
		printf("Error");
	}

	return EXIT_SUCCESS;
}
