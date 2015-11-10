/*
 * test.c
 *
 *  Created on: 9 de nov. de 2015
 *      Author: admin
 */
#include <stdio.h>
#include <stdlib.h>
#include "ecrypt-sync.h"
#include <string.h>
#include <math.h>

void test() {
	int tamanio = 53 * sizeof(u8);
	int tamanio_escritura = tamanio_archivo("C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen.bmp") - tamanio;
	FILE *archivo = fopen("C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen.bmp", "rb");
	FILE *salida = fopen("C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen2.bmp", "wb");
	FILE *salida_medio = fopen("C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen12.bmp", "wb");

	u8 iv = 0;
	ECRYPT_ctx * ctx = malloc(sizeof(ECRYPT_ctx));
	inicia_ctx(ctx);
	char * encabezado = malloc(tamanio);
	fread(encabezado, tamanio, 1, archivo);

	char * lector = malloc(tamanio_escritura);
	char * output = malloc(tamanio_escritura);
	char * temporal = malloc(tamanio_escritura);
	char * original = malloc(tamanio_escritura);
	char * impresion = malloc(tamanio_escritura + tamanio);
	char * intermedio = malloc(tamanio_escritura + tamanio);
	fread(lector, tamanio_escritura, 1, archivo);
	ECRYPT_process_packet(0, ctx, &iv, lector, output, tamanio_escritura);
	memcpy(intermedio, encabezado, tamanio);
	memcpy(intermedio + tamanio, output, tamanio_escritura);
	memcpy(temporal, output, tamanio_escritura);
	fwrite(intermedio,tamanio + tamanio_escritura, 1, salida_medio);

	inicia_ctx(ctx);
	iv = 0;

	ECRYPT_process_packet(1, ctx, &iv, output, original, tamanio_escritura);
	memcpy(impresion, encabezado, tamanio);
	memcpy(impresion + tamanio, original, tamanio_escritura);
	fwrite(impresion,tamanio + tamanio_escritura, 1, salida);

	fclose(salida);
	fclose(salida_medio);
	fclose(archivo);

	salida_medio = fopen("C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen12.bmp","rb");
	salida = fopen("C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen3.bmp", "wb");
	free(encabezado);
	free(output);
	free(original);
	free(impresion);
	output = malloc(tamanio_escritura);
	original = malloc(tamanio_escritura);
	impresion = malloc(tamanio_escritura + tamanio);
	encabezado = malloc(tamanio);
	fread(encabezado, tamanio, 1, salida_medio);
	fread(output, tamanio_escritura, 1, archivo);

	inicia_ctx(ctx);
	iv = 0;

	ECRYPT_process_packet(1, ctx, &iv, output, original, tamanio_escritura);
	memcpy(impresion, encabezado, tamanio);
	memcpy(impresion + tamanio, original, tamanio_escritura);
	fwrite(impresion,tamanio + tamanio_escritura, 1, salida);

	int i =0, resultado = 0;
	char c_original, c_final;
	while (i< tamanio_escritura && resultado == 0){
		c_original = temporal[i];
		c_final = original[i];
		i++;
		if (c_original != c_final){
			resultado = 1;
			printf("Byte del fallo: %d de %d \n", i, tamanio_escritura);
		}
	}

	printf("TEST: ");
	if (comprueba("C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen.bmp", "C:\\Users\\admin\\git\\utncrypto2015\\src\\imagen3.bmp")==0){
		puts("Proceso Correcto");
	} else {
		puts("ERROR");
	}


}
