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
#include <string.h>

int main(int argc, char **argv) {

	int accion = 0;
	ECRYPT_ctx * ctx = malloc(sizeof(ECRYPT_ctx));
	u8 * iv = malloc(sizeof(u8));
	u32 msglen;
	FILE *archivo;
	FILE *archivo_salida;
	FILE *archivo_salida_decrypt;
	u8 lector;
	int inicio_iv = 0;

	char* imagen = "/home/utnso/git/utncrypto2015/src/imagen.bmp";
	char* imagen_encrypt =
			"/home/utnso/git/utncrypto2015/src/imagen_encrypt.bmp";
	char* imagen_decrypt =
			"/home/utnso/git/utncrypto2015/src/imagen_decrypt.bmp";

	archivo = fopen(imagen, "rb");
	archivo_salida = fopen(imagen_encrypt, "w");
	archivo_salida_decrypt = fopen(imagen_decrypt, "w");

	if (!archivo) {
		printf("Imposible arbrir el archivo!");
		return 1;
	}

	int tamanio = 50 * sizeof(char);
	char encabezado[tamanio];

	fgets(encabezado, tamanio, archivo);

	int i = 0;
	for (i = 0; i < tamanio; i++) {
		fputc(encabezado[i], archivo_salida);
		fputc(encabezado[i], archivo_salida_decrypt);
	}

	*iv = inicio_iv;
	u8 * input = malloc(sizeof(u8));
	u8 * output = malloc(sizeof(u8));
	u8 * output2 = malloc(sizeof(u8));

	while (feof(archivo) == 0) {
		fread(&lector, sizeof(u8), 1, archivo);
		*input = lector;
		msglen = sizeof(u8);

		// encripto
		ECRYPT_process_packet(accion, ctx, iv, input, output, msglen);

		//creo el archivo
		fwrite(output, sizeof(u8), 1, archivo_salida);
	}

	free(ctx);
	fclose(archivo);
	fclose(archivo_salida);

	archivo_salida = fopen(imagen_encrypt, "rb");

	//Desencripto
	accion = 1;
	*iv = inicio_iv;
	ctx = malloc(sizeof(ECRYPT_ctx));

	fgets(encabezado, tamanio, archivo_salida);

	while (feof(archivo_salida) == 0) {

		fread(&lector, sizeof(u8), 1, archivo_salida);
		*input = lector;
		msglen = sizeof(u8);

		ECRYPT_process_packet(accion, ctx, iv, input, output, msglen);

		//creo el archivo
		fwrite(output, sizeof(u8), 1, archivo_salida_decrypt);

	}

	free(input);
	free(output);
	free(ctx);
	fclose(archivo_salida);
	fclose(archivo_salida_decrypt);

	return EXIT_SUCCESS;
}
