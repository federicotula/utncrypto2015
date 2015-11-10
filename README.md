# utncrypto2015 - Rabbit

# Metodo de uso

1. Cargar y compilar con eclipse
2. Abrir una consola de comandos con el programa compilado y pasarle alguna combinacion de los siguientes parametros
  * **Para ENCRIPTAR** <br>
    `utncrypto2015.exe 0 path_imagen_original path_imagen_encriptada path_configuracion_ctx` <br><br>
    Encripta la imagen original con la configuracion de ctx seleccionada y crea la imagen encriptada en el lugar seleccionado<br>
  * **Para DECRIPTAR** <br>
    `utncrypto2015.exe 1 path_imagen_encriptada path_imagen_destino path_configuracion_ctx` <br><br>
    Decripta la imagen encriptada con la configuracion de ctx seleccionada y crea la imagen destino <br>
  * **Para comprobar imagenes** <br>
    `utncrypto2015.exe 2 path_imagen_A path_imagen_B` <br><br>
    Comprueba que las dos imagenes sean iguales. <br>
    Es probable que de un error en el Byte 54/55 es porque lo anterior es el encabezado <br>
 <br>  

## Aclaracion
El archivo de configuracion de CTX tiene que tener la siguiente estructura
```xml
  <master_c>1,2,3,4,5,6,7,8</master_c>
  <master_x>1,2,3,4,5,6,7,8</master_x>
  <master_carry>1</master_carry>
  <work_c>1,2,3,4,5,6,7,8</work_c>
  <work_x>1,2,3,4,5,6,7,8</work_x>
  <work_carry>1</work_carry>
```
