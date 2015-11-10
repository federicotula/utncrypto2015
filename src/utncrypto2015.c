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
int comprueba(char * path_original, char * path_final);

/*
int main(int argc, char **argv) {

	int accion;

	char * path_imagen = "C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen.bmp";
	char * path_encryptada = "C:\\Users\\admin\\git\\utncrypto2015\\src\\e_imagen.bmp";
	char * path_decryptada = "C:\\Users\\admin\\git\\utncrypto2015\\src\\d_imagen.bmp";

	accion = 0;
	cifrador(accion, path_imagen, path_encryptada);

	accion = 1;
	cifrador(accion, path_encryptada, path_decryptada);

	printf("El resultado del proceso es: ");
	if (comprueba(path_imagen, path_decryptada)==0){
		printf("Proceso Correcto \n");
	} else {
		printf("ERROR \n");
	}

	return EXIT_SUCCESS;
}
*/

int main(int argc, char **argv) {

	int accion = argv[1][0] - 48; // Paso del codigo ASCII a decimal

	if (argc != 3){
		printf("Cifrador Rabbit \n");

		switch (accion) {
			case 0:
				printf("Seleccionaste Cifrar \n");
				printf("El archivo de ORIGEN es: %s \n", argv[2]);
				printf("El archivo de DESTINO es: %s \n", argv[3]);

				cifrador(CIFRAR, argv[2], argv[3]);
				break;
			case 1:
				printf("Seleccionaste Decifrar \n");
				printf("El archivo de ORIGEN es: %s \n", argv[2]);
				printf("El archivo de DESTINO es: %s \n", argv[3]);

				cifrador(DECIFRAR, argv[2], argv[3]);
				break;
			case 2:
				printf("Seleccionaste Comprobar cifrado \n");
				printf("El archivo de ORIGINAL es: %s \n", argv[2]);
				printf("El archivo de DECIFRADO es: %s \n", argv[3]);

				printf("El resultado del proceso es: ");
				if (comprueba(argv[2], argv[3])==0){
					printf("Proceso Correcto \n");
				} else {
					printf("ERROR \n");
				}
				break;
			default:
				break;
		}
	}

	return EXIT_SUCCESS;
}


int comprueba(char * path_original, char * path_final){
	int resultado = 0, i=0, tamanio = tamanio_archivo(path_original);
	FILE *original = fopen(path_original, "rb");
	FILE *final = fopen(path_final, "rb");
	char c_original, c_final;

	while (!feof(original) && resultado == 0 && i< tamanio){
		c_original = fgetc(original);
		c_final = fgetc(final);
		i++;
		if (c_original != c_final){
			resultado = 1;
			printf("Byte del fallo: %d \n", i);
		}
	}

	fclose(original);
	fclose(final);

	return resultado;

}

long int tamanio_archivo(char* path) {

	FILE * fichero;
	long int tamanio = 0;

	fichero = fopen(path, "rb");

	while (!feof(fichero)) {
		fgetc(fichero);
		tamanio++;
	}
	tamanio--;
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
	u8 * iv = malloc(sizeof(u8));
	*iv = 0;
	u32 tamanio_funcion, tamanio = 53 * sizeof(u8);

	// Inicio la clave ctx
	ECRYPT_ctx * ctx = malloc(sizeof(ECRYPT_ctx));
	inicia_ctx(ctx);

	// Abro los archivos y verifico que existan
	archivo_origen = fopen(path_origen, "rb");
	archivo_destino = fopen(path_destino, "wb");

	if (!archivo_origen) {
		printf("Imposible arbrir el archivo!");
		system("exit");
	}

	if (!archivo_destino) {
		printf("Imposible arbrir el archivo!");
		system("exit");
	}

	// Tamaño de lo que se va a pasar por la funcion
	tamanio_funcion = tamanio_archivo(path_origen) - tamanio;
	printf("Tamaño: %d  \n", tamanio_funcion)

	u8 * encabezado = malloc(tamanio);
	u8 * impresion = malloc(tamanio_funcion + tamanio);
	u8 * lector = malloc(tamanio_funcion);
	u8 * output = malloc(tamanio_funcion);

	// Limpio el output que es lo que fallaba
	u32 i = 0;
	for(i=0;i<tamanio_funcion;i++){
		lector[i] = 0;
		output[i] = 0;
	}

	// Obtengo encabezado y contenido
	fread(encabezado, tamanio, 1, archivo_origen);
	fread(lector, tamanio_funcion, 1, archivo_origen);

	// Proceso lo que obtengo en la lectura
	ECRYPT_process_packet(accion, ctx, iv, lector, output, tamanio_funcion);

	// Paso a una unica direccion de memoria para hacer una sola escritura
	memcpy(impresion, encabezado, tamanio);
	memcpy(impresion + tamanio, output, tamanio_funcion);
	fwrite(impresion, tamanio + tamanio_funcion, 1, archivo_destino);


	//Libero memoria
	free(output);
	free(lector);
	free(impresion);
	free(encabezado);
	fclose(archivo_destino);
	fclose(archivo_origen);
	free(ctx);

}
