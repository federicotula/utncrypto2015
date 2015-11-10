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
void inicia_ctx(ECRYPT_ctx * ctx, char * path);
void cifrador(int accion, FILE * archivo_origen, FILE * archivo_destino,
		ECRYPT_ctx * ctx, int tamanio_bloque);
int comprueba(char * path_original, char * path_final);
char * conseguir_extension(char * path_origen);
void MinToMay(char string[]);
void cifrador_archivo(int accion, char * path_origen, char * path_destino, ECRYPT_ctx * ctx);

int main(int argc, char **argv) {

	int accion = 0;
	if (argc > 1)
		accion = argv[1][0] - 48; // Paso del codigo ASCII a decimal

	if (argc == 5 || (argc == 4 && accion == 2)) {
		printf("Cifrador Rabbit \n");

		// Inicio la clave ctx
		ECRYPT_ctx * ctx = malloc(sizeof(ECRYPT_ctx));

		switch (accion) {
		case 0:									//CIFRAR
			printf("Seleccionaste Cifrar \n");
			printf("El archivo de ORIGEN es: %s \n", argv[2]);
			printf("El archivo de DESTINO es: %s \n", argv[3]);
			
			printf( "     /)_/)                                       \n");
			printf( "    ( ^.^)     ¡Cifrado correcto!                \n");
			printf( "    c(\")(\")                                    \n");

			inicia_ctx(ctx, argv[4]);
			cifrador_archivo(CIFRAR, argv[2], argv[3], ctx);
			break;
		case 1:									//DECIFRAR
			printf("Seleccionaste Decifrar \n");
			printf("El archivo de ORIGEN es: %s \n", argv[2]);
			printf("El archivo de DESTINO es: %s \n", argv[3]);
			
			printf( "     /)_/)                                       \n");
			printf( "    ( ^.^)     ¡Decifrado correcto!              \n");
			printf( "    c(\")(\")                                    \n");

			inicia_ctx(ctx, argv[4]);
			cifrador_archivo(DECIFRAR, argv[2], argv[3], ctx);
			break;
		case 2:									//COMPROBAR IMAGENES
			printf("Seleccionaste Comprobar cifrado \n");
			printf("El archivo de ORIGINAL es: %s \n", argv[2]);
			printf("El archivo de DECIFRADO es: %s \n", argv[3]);
			if (comprueba(argv[2], argv[3]) == 0) {
				printf( "     /)_/)                                       \n");
				printf( "    ( ^.^)     ¡Comprobación correcta!           \n");
				printf( "    c(\")(\")                                    \n");
			} else {
				printf( "     /)_/)                                       \n");
				printf( "    ( v.v)     ¡Comprobación incorrecta!         \n");
				printf( "    c(\")(\")                                    \n");
			}
			break;
		default:
			break;
		}
	}

	return EXIT_SUCCESS;
}

int comprueba(char * path_original, char * path_final) {
	// Comprueba que los dos archivos enviados en los path sean identicos

	// Es normal que de como resultado "Byte del fallo: 54" porque es donde
	// empieza el archivo y termina el encabezado

	int resultado = 0, i = 0, tamanio = tamanio_archivo(path_original);
	FILE *original = fopen(path_original, "rb");
	FILE *final = fopen(path_final, "rb");
	char c_original, c_final;

	while (!feof(original) && resultado == 0 && i < tamanio) {
		c_original = fgetc(original);
		c_final = fgetc(final);
		i++;
		if (c_original != c_final) {
			resultado = 1;
			printf("Byte del fallo: %d \n", i);
		}
	}

	fclose(original);
	fclose(final);

	return resultado;

}

char * conseguir_extension(char * path_origen) {

	// A partir del path obtengo la extension del archivo origen
	// Lo devuelvo siempre en mayusculas

	char * extension;
	int i = 0, fin;
	while (path_origen[i] != '\0') i++;
	fin = i;
	for (; path_origen[i] != '.'; i--) ;
	i++;
	extension = malloc(fin - i);
	memcpy(extension, path_origen + i, fin - i);

	extension[fin - i] = '\0';

	MinToMay(extension);

	return extension;
}

void MinToMay(char string[]){
	int i=0;
	int desp='a'-'A';
	for (i=0;string[i]!='\0';++i)
	{
		if(string[i]>='a'&&string[i]<='z')
		{
			string[i]=string[i]-desp;
		}
	}
}

long int tamanio_archivo(char* path) {

	// Devuelve el tamaño del archivo, incluyendo el encabezado
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

void inicia_ctx(ECRYPT_ctx * ctx, char * path_ctx) {

	// Cargo la configuracion para el registro ctx para que no este
	// hardcodeada la informacion para encriptar

	int i, t = sizeof(u32);
	FILE * ctx_config = fopen(path_ctx, "rb");
	u32 master_c[8 * t], master_x[8 * t], work_c[8 * t], work_x[8 * t];
	u32 master_carry, work_carry;

	fscanf(ctx_config, "<master_c>%u,%u,%u,%u,%u,%u,%u,%u</master_c>\n",
			&master_c[0], &master_c[1], &master_c[2], &master_c[3],
			&master_c[4], &master_c[5], &master_c[6], &master_c[7]);
	fscanf(ctx_config, "<master_x>%u,%u,%u,%u,%u,%u,%u,%u</master_x>\n",
			&master_x[0], &master_x[1], &master_x[2], &master_x[3],
			&master_x[4], &master_x[5], &master_x[6], &master_x[7]);
	fscanf(ctx_config, "<master_carry>%u</master_carry>\n", &master_carry);
	fscanf(ctx_config, "<work_c>%u,%u,%u,%u,%u,%u,%u,%u</work_c>\n", &work_c[0],
			&work_c[1], &work_c[2], &work_c[3], &work_c[4], &work_c[5],
			&work_c[6], &work_c[7]);
	fscanf(ctx_config, "<work_x>%u,%u,%u,%u,%u,%u,%u,%u</work_x>\n", &work_x[0],
			&work_x[1], &work_x[2], &work_x[3], &work_x[4], &work_x[5],
			&work_x[6], &work_x[7]);
	fscanf(ctx_config, "<work_carry>%u</work_carry>", &work_carry);

	for (i = 0; i < 8; i++) {
		ctx->master_ctx.c[i] = master_c[i];
		ctx->master_ctx.x[i] = master_x[i];
		ctx->work_ctx.c[i] = work_c[i];
		ctx->work_ctx.x[i] = work_x[i];
	}
	ctx->master_ctx.carry = (u32) master_carry;
	ctx->work_ctx.carry = (u32) work_carry;
}

void copiar_texto_claro(FILE * archivo_origen, FILE * archivo_destino, int tamanio){
	u8 * texto = malloc(tamanio);
	fread(texto, tamanio, 1, archivo_origen);
	fwrite(texto, tamanio, 1, archivo_destino);
	free(texto);
}

void cifrador_archivo(int accion, char * path_origen, char * path_destino, ECRYPT_ctx * ctx){

	FILE * archivo_origen;
	FILE * archivo_destino;

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

	char * extension = conseguir_extension(path_origen);

	if (strcmp(extension, "BMP") == 0) {
		long int encabezado = 53;
		long int total_archivo = tamanio_archivo(path_origen);
		copiar_texto_claro( archivo_origen, archivo_destino, encabezado);
		cifrador(accion, archivo_origen, archivo_destino, ctx, total_archivo- encabezado);
	} else if(strcmp(extension, "JPG") == 0){
		long int encabezado = 30;
		long int pie = 30;
		long int total_archivo = tamanio_archivo(path_origen);
		copiar_texto_claro( archivo_origen, archivo_destino, encabezado);
		cifrador(accion, archivo_origen, archivo_destino, ctx, total_archivo- encabezado- pie);
		copiar_texto_claro( archivo_origen, archivo_destino, pie);
	}



	fclose(archivo_destino);
	fclose(archivo_origen);
	free(ctx);

}

void cifrador(int accion, FILE * archivo_origen, FILE * archivo_destino,
		ECRYPT_ctx * ctx, int tamanio_bloque) {

	u8 * lector = malloc(tamanio_bloque);
	u8 * output = malloc(tamanio_bloque);

	// Limpio el output que es lo que fallaba
	u32 i = 0;
	for (i = 0; i < tamanio_bloque; i++) {
		lector[i] = 0;
		output[i] = 0;
	}

	// Obtengo el contenido
	fread(lector, tamanio_bloque, 1, archivo_origen);

	// Proceso lo que obtengo en la lectura
	//ECRYPT_process_packet(accion, ctx, &iv, lector, output, tamanio_bloque);
	ECRYPT_process_bytes(accion, ctx, lector, output, tamanio_bloque);

	// Paso a una unica direccion de memoria para hacer una sola escritura
	fwrite(output, tamanio_bloque, 1, archivo_destino);

	//Libero memoria
	fflush(archivo_destino);
	fflush(archivo_origen);
	free(output);
	free(lector);
}
