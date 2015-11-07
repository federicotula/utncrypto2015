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
#include <math.h>

const int CIFRAR = 0;
const int DECIFRAR = 1;

long int tamanio_archivo(char* path);
void inicia_ctx(ECRYPT_ctx * ctx);
void cifrador(int accion, char * path_origen, char * path_destino);

int main(int argc, char **argv) {

	int accion = 0;

	if (argv[1][0] == 48){
		accion = 0;
	} else {
		accion = 1;
	}

	cifrador(accion, argv[2], argv[3]);

/*
	char* imagen = "C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen.bmp";
	char* imagen_encrypt =
			"C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen_encrypt.bmp";
	char* imagen_decrypt =
			"C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen_decrypt.bmp";


	// CIFRO
	cifrador(CIFRAR, imagen, imagen_encrypt);

	// DECIFRO
	cifrador(DECIFRAR, imagen_encrypt, imagen_decrypt);
*/
	return EXIT_SUCCESS;
}

long int tamanio_archivo(char* path) {

	FILE * fichero;
	double long tamanio = 0;

	fichero = fopen(path, "r");
	fseek(fichero, 0L, SEEK_END);

	tamanio = ftell(fichero);

	fclose(fichero);

	return tamanio;
}

void inicia_ctx(ECRYPT_ctx * ctx) {

	int i;

	for (i = 0; i < 8; i++) {
		ctx->master_ctx.c[i] = 0;
		ctx->master_ctx.x[i] = 0;
		ctx->work_ctx.c[i] = 0;
		ctx->work_ctx.x[i] = 0;
	}
	ctx->master_ctx.carry = 0;
	ctx->work_ctx.carry = 0;
}

void cifrador(int accion, char * path_origen, char * path_destino) {

	FILE *archivo_origen;
	FILE *archivo_destino;
	u8 iv = 0;
	ECRYPT_ctx * ctx = malloc(sizeof(ECRYPT_ctx));
	inicia_ctx(ctx);

	int tamanio = 53 * sizeof(char);
	char encabezado[tamanio];

	archivo_origen = fopen(path_origen, "rb");
	archivo_destino = fopen(path_destino, "w");

	if (!archivo_origen) {
		printf("Imposible arbrir el archivo!");
		system("exit");
	}

	fgets(encabezado, tamanio, archivo_origen);
	fwrite(encabezado, tamanio, 1, archivo_destino);

	long int tamanio_lectura = tamanio_archivo(path_origen) - tamanio;
	long int tamanio_escritura;

	if (accion == 0) {
		tamanio_escritura = tamanio_lectura * 3;
	} else {
		tamanio_escritura = (long int) floor(tamanio_lectura/3);
	}

	u8 * lector = malloc(tamanio_lectura);
	u8 * output = malloc(tamanio_escritura);

	fread(lector, tamanio_lectura, 1, archivo_origen);
	//fgets(lector, tamanio_original, archivo_origen);

	ECRYPT_process_packet(accion, ctx, &iv, lector, output, tamanio_lectura);

	//creo el archivo
	fwrite(output, tamanio_escritura, 1, archivo_destino);

	free(output);
	free(lector);
	free(ctx);
	fclose(archivo_origen);
	fclose(archivo_destino);

}

/*
	while (feof(archivo_origen) == 0) {
		fread(lector, tamanio_lectura, 1, archivo_origen);
		ECRYPT_process_packet(accion, ctx, &iv, lector, output, tamanio_lectura);
		fwrite(output, tamanio_escritura, 1, archivo_destino);
	}
 */
