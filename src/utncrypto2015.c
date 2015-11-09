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
void test();
int comprueba(char * path_original, char * path_final);


int main(int argc, char **argv) {
	/*
	 int accion = 0;

	 if (argv[1][0] == 48){
	 accion = 0;
	 } else {
	 accion = 1;
	 }

	 cifrador(accion, argv[2], argv[3]);
	 */

	char* imagen = "C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen.bmp";
	char* imagen_encrypt = "C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen_e.bmp";
	char* imagen_decrypt = "C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen_d.bmp";


	//test();
	// CIFRO
	cifrador(CIFRAR, imagen, imagen_encrypt);

	// DECIFRO
	cifrador(DECIFRAR, imagen_encrypt, imagen_decrypt);

	printf("VERDADERO: ");
	if (comprueba(imagen, imagen_decrypt)==0){
		puts("Proceso Correcto");
	} else {
		puts("ERROR");
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
		}
	}

	fclose(original);
	fclose(final);

	return resultado;

}

void test() {
	int tamanio = 53 * sizeof(u8);
	int tamanio_lectura = tamanio_archivo("C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen.bmp") - tamanio;
	int tamanio_escritura = tamanio_lectura ;
	FILE *archivo = fopen("C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen.bmp", "rb");
	FILE *salida = fopen("C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen2.bmp", "wb");
	FILE *salida_medio = fopen("C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen12.bmp", "wb");

	u8 iv = 0;
	ECRYPT_ctx * ctx = malloc(sizeof(ECRYPT_ctx));
	inicia_ctx(ctx);
	char * encabezado = malloc(tamanio);
	fread(encabezado, tamanio, 1, archivo);

	char * lector = malloc(tamanio_lectura);
	char * output = malloc(tamanio_escritura);
	char * original = malloc(tamanio_lectura);
	char * impresion = malloc(tamanio_lectura + tamanio);
	char * intermedio = malloc(tamanio_escritura + tamanio);
	fread(lector, tamanio_lectura, 1, archivo);
	ECRYPT_process_packet(0, ctx, &iv, lector, output, tamanio_lectura);
	memcpy(intermedio, encabezado, tamanio);
	memcpy(intermedio + tamanio, output, tamanio_escritura);
	fwrite(intermedio,tamanio + tamanio_escritura, 1, salida_medio);

	inicia_ctx(ctx);
	iv = 0;

	ECRYPT_process_packet(1, ctx, &iv, output, original, tamanio_lectura);
	memcpy(impresion, encabezado, tamanio);
	memcpy(impresion + tamanio, original, tamanio_lectura);
	fwrite(impresion,tamanio + tamanio_lectura, 1, salida);

	fclose(salida);
	fclose(salida_medio);
	fclose(archivo);

	printf("TEST: ");
	if (comprueba("C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen.bmp", "C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen2.bmp")==0){
		puts("Proceso Correcto");
	} else {
		puts("ERROR");
	}


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
	u8 iv = 0;
	ECRYPT_ctx * ctx = malloc(sizeof(ECRYPT_ctx));
	inicia_ctx(ctx);
	long int tamanio_escritura, tamanio_lectura;

	int tamanio = 53 * sizeof(u8);

	archivo_origen = fopen(path_origen, "rb");
	archivo_destino = fopen(path_destino, "w");

	if (!archivo_origen) {
		printf("Imposible arbrir el archivo!");
		system("exit");
	}

	if (!archivo_destino) {
		printf("Imposible arbrir el archivo!");
		system("exit");
	}

	tamanio_lectura = tamanio_archivo(path_origen) - tamanio;
	if (accion == 0) {
		tamanio_escritura = tamanio_lectura * 3;
	} else {
		tamanio_escritura = (long int) floor(tamanio_lectura / 3);
	}

	char * encabezado = malloc(tamanio);
	char * impresion = malloc(tamanio_escritura + tamanio);
	char * lector = malloc(tamanio_lectura);
	char * output = malloc(tamanio_escritura);

	fread(encabezado, tamanio, 1, archivo_origen);
	fread(lector, tamanio_lectura, 1, archivo_origen);

	if (accion == 0) {
		ECRYPT_process_packet(accion, ctx, &iv, lector, output, tamanio_lectura);
	} else {
		ECRYPT_process_packet(accion, ctx, &iv, lector, output, tamanio_escritura);
	}

	//creo el archivo
	memcpy(impresion, encabezado, tamanio);
	memcpy(impresion + tamanio, output, tamanio_escritura);
	fwrite(impresion, tamanio_escritura + tamanio, 1, archivo_destino);

	free(encabezado);
	free(output);
	free(lector);
	free(ctx);
//	fflush(archivo_destino);
//	fflush(archivo_origen);
	fclose(archivo_destino);
	fclose(archivo_origen);

}

/*
 while (feof(archivo_origen) == 0) {
 fread(lector, tamanio_lectura, 1, archivo_origen);
 ECRYPT_process_packet(accion, ctx, &iv, lector, output, tamanio_lectura);
 fwrite(output, tamanio_escritura, 1, archivo_destino);
 }
 */
