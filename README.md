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

Es necesario contar con lo siguiente para poder ejecutar el programa:

- Compilador de C (gcc es el que fue usado).
- Libreria cairo (cairo.h & cairo-xlib.h).

```bash
  sudo apt install libcairo2-dev
```

- Libreria Xlib (X11/XKBlib.h).

```bash
  sudo apt install libx11-dev
```

**\*Nota:** Las demás librerias utilizadas deberian venir preinstaladas, pero si no fuera el caso es necesario contar con toda la siguiente lista:\*

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
  //Libreria Xlib
  #include <X11/XKBlib.h>
  //Libreria cairo
  #include <cairo.h>
  #include <cairo-xlib.h>
```

# Instrucciones de uso:

Una vez teniendo el archivo **main.c** ejecutar el siguiente comando para compilarlo:

```bash
  gcc main.c -o main `pkg-config --cflags --libs cairo x11`
```

y despues se ejecuta de la siguiente manera:

```bash
  sudo ./main
```

Y automaticamente despues de ejecutar eso se nos creara un TXT en la ruta del ejecutable **main** que empezará a registrar las teclas que vamos pulsando además de tomar capturas de pantalla **cada minuto**.
