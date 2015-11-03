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

long int tamanio_archivo(char* path);

int main(int argc, char **argv) {

	int accion = 0;
	ECRYPT_ctx * ctx = malloc(sizeof(ECRYPT_ctx));
	u8 * iv = malloc(sizeof(u8));
	FILE *archivo;
	FILE *archivo_encrypt;
	FILE *archivo_decrypt;
	int inicio_iv = 0;
	int tamanio = 53 * sizeof(char);
	char encabezado[tamanio+1];

	char* imagen = "C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen.bmp";
	char* imagen_encrypt =
			"C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen_encrypt.bmp";
	char* imagen_decrypt =
			"C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen_decrypt.bmp";

	archivo = fopen(imagen, "rb");
	archivo_encrypt = fopen(imagen_encrypt, "w");
	archivo_decrypt = fopen(imagen_decrypt, "w");

	if (!archivo) {
		printf("Imposible arbrir el archivo!");
		return 1;
	}


	int i = 0;
	fgets(encabezado, tamanio, archivo);
	for (i = 0; i < tamanio; i++) {
		fputc(encabezado[i], archivo_encrypt);
	}

	*iv = inicio_iv;

	long int tamanio_original = tamanio_archivo(imagen) - tamanio;
	u8 * lector = malloc(tamanio_original + 1);
	fread(lector, tamanio_original, 1, archivo);
	u8 * output = malloc(tamanio_original + 1);

	ECRYPT_process_packet(accion, ctx, iv, lector, output, tamanio_original);

	//creo el archivo
	fwrite(output, tamanio_original, 1, archivo_encrypt);

	free(lector);
	free(output);
	free(encabezado);
	free(ctx);
	fclose(archivo);
	fclose(archivo_encrypt);



	//Desencripto
	archivo_encrypt = fopen(imagen_encrypt, "rb");

	fgets(encabezado, tamanio, archivo_encrypt);

	for (i = 0; i < tamanio; i++) {
		fputc(encabezado[i], archivo_decrypt);
	}

	accion = 1;
	*iv = inicio_iv;
	ctx = malloc(sizeof(ECRYPT_ctx));

	long int tamanio_encrypt = tamanio_archivo(imagen_encrypt) - tamanio;
	lector = malloc(tamanio_encrypt + 1);
	fread(lector, tamanio_encrypt, 1, archivo_encrypt);
	output = malloc(tamanio_encrypt + 1);

	ECRYPT_process_packet(accion, ctx, iv, lector, output, tamanio_encrypt);

	//creo el archivo
	fwrite(output, tamanio_encrypt, 1, archivo_decrypt);

	free(output);
	free(ctx);
	free(encabezado);

	fclose(archivo_encrypt);
	fclose(archivo_decrypt);

	return EXIT_SUCCESS;
}

long int tamanio_archivo(char* path){

	FILE * fichero;
	double long tamanio = 0;

	fichero = fopen(path,"r");
	fseek(fichero, 0L, SEEK_END);

	tamanio = ftell(fichero);

	fclose(fichero);

	return tamanio;
}

/*
	while (feof(archivo) == 0) {
		fread(&lector, sizeof(u8), 1, archivo);
		*input = lector;
		msglen = sizeof(u8);

		// encripto
		ECRYPT_process_packet(accion, ctx, iv, input, output, msglen);

		//creo el archivo
		fwrite(output, sizeof(u8), 1, archivo_encrypt);
	}
*/

/*
	while (feof(archivo_encrypt) == 0) {

		fread(&lector, sizeof(u8), 1, archivo_encrypt);
		*input = lector;
		msglen = sizeof(u8);

		ECRYPT_process_packet(accion, ctx, iv, input, output2, msglen);

		//creo el archivo
		fwrite(output2, sizeof(u8), 1, archivo_decrypt);

	}

	free(input);
*/
