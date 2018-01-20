/*
 * Sistemas operativos empotrados
 * Implementación de las llamadas asl sistema
 */

#include <sys/types.h>
#include <reent.h>
#include <errno.h>

#include "system.h"

/*****************************************************************************/

/**
 * Dirección de inicio del heap, definida en el script de enlazado
 */
extern void _heap_start, _heap_end;

/*****************************************************************************/

/**
 * Incrementa el tamaño de área reservada para datos dentro del heap.
 * Las funciones de gestión de memoria dinamica (malloc) depende de esta llamada al sistema.
 * @param incr  Tamaño del incremento solicitado (en bytes)
 * @return      Puntero al nuevo bloque de memoria asignado en caso de éxito
 *              -1 en caso de error
 *              La condición de error se indica en la variable global errno
 */
void * _sbrk (intptr_t incr){
    static void * current_break = &_heap_start;
    void * last_break = current_break;

    /* Anulamos las interrupciones durante el proceso de reserva */
    /* Comienzo de la sección crítica */
    itc_disable_ints();

    /* Forzamos a que el incremento sea un múltiplo del tamaño de la palabra */
    incr = (intptr_t) (((unsigned int)incr + 3) & ~3);

    /* Comprobamos que hay memoria suficiente en el heap */
    if (current_break + incr > &_heap_end){
        errno = ENOMEM;
        last_break = (void *) -1;
    }
    else{
        /* Ampliamos el área reservada para datos dinámicos */
        current_break += incr;
    }

    /* Volvemos a habilitar las interrupciones */
    /* Fin de la sección crítica */
    itc_restore_ints();

    return last_break;
}

/*****************************************************************************/

/**
 * Abre un dispositivo/fichero
 * @param pathname  Nombre del dispositivo/fichero
 * @param flags     Modo de acceso
 * @param mode      Permisos en caso de que se cree un archivo/dispositivo
 * @return          Descriptor del dispositivo en caso de éxito
 *                  -1 en caso de error
 *                  La condición de error se indica en la variable global errno
 */
int _open(const char * pathname, int flags, mode_t mode){
    bsp_dev_t * dev = find_dev (pathname);      /* Buscamos el dispositivo en la tabla de dispositivos del BSP */

    /* Si el dispositivo existe */
    if (dev != NULL){
        /*
        * Si tiene implementada la función open y no falla al llamarla, se le asigna un descriptor
        * Si no la tiene, como por defecto se puede abrir cualquier dispositivo, también se le asigna un descriptor
        */
        if (dev->open==NULL || dev->open(dev->id, flags, mode) >= 0){
            return get_fd(dev, flags);
        }
    }

    /* Si el dispositivo no existe */
    else
        errno = ENODEV;     /* Ajustamos el valor de errno */
    }
    

    return -1;
}

/*****************************************************************************/

/**
 * Cierra un dispositivo/fichero
 * @param fd    Descriptor de fichero/dispositivo
 * @return      0 en caso de éxito
 *              -1 en caso de error
 *              La condición de error se indica en la variable global errno
 */
int _close (int fd){
    release_fd(fd);                     /* Liberamos el descriptor de fichero */

    bsp_dev_t * dev = get_dev (fd);     /* Buscamos el dispositivo en la tabla de dispositivos del BSP */

    /* Si el dispositivo existe y tiene implementada la función close, devolvemos su salida */
    if (dev != NULL && dev->close != NULL){
        return dev->close(dev->id);
    }

    /* Si no, fd no es un descriptor válido */
    else{
        errno = EBADF;      /* Ajustamos el valor de errno */

        return -1;
    }
}

/*****************************************************************************/

/**
 * Lectura de un dispositivo/fichero
 * @param fd    Descriptor de fichero/dispositivo
 * @param buf   Puntero al búfer donde se almacenarán los datos
 * @param count Número de bytes que se quieren leer
 * @return      Número de bytes leídos
 *              -1 en caso de error
 *              La condición de error se indica en la variable global errno
 */
ssize_t _read(int fd, char *buf, size_t count){
	bsp_dev_t * dev = get_dev (fd);     /* Buscamos el dispositivo en la tabla de dispositivos del BSP */

    /* Si el dispositivo existe y tiene implementada la función read, devolvemos su salida */
    if (dev != NULL && dev->read != NULL){
        return dev->read(dev->id, buf, count);
    }

    /* En otro caso */
    else{
        return 0;           /* Devolvemos 0, es decir, que no se ha podido leer ningún byte */
    }
}

/*****************************************************************************/

/**
 * Escritura en un dispositivo/fichero
 * @param fd    Descriptor de fichero/dispositivo
 * @param buf   Puntero al búfer que almacena los datos
 * @param count Número de bytes que se quieren escribir
 * @return      Número de bytes escritos en caso de éxito
 *              -1 en caso de error
 *              La condición de error se indica en la variable global errno
 */
ssize_t _write (int fd, char *buf, size_t count){
	bsp_dev_t * dev = get_dev (fd);     /* Buscamos el dispositivo en la tabla de dispositivos del BSP */

    /* Si el dispositivo existe y tiene implementada la función write, devolvemos su salida */
    if (dev != NULL && dev->write != NULL){
        return dev->write(dev->id, buf, count);
    }

    /* En otro caso */
    else{
        return count;           /* Devolvemos el total, claramente hemos escrito todo (MENTIRA!!!) */
    }
}

/*****************************************************************************/

/**
 * Modificación del desplazamiento en un dispositivo/fichero
 * @param fd        Descriptor de fichero/dispositivo
 * @param offset    Desplazamiento
 * @param whence    Base para el desplazamiento
 * @return          Nuevo desplazamiento (en bytes) desde el inicio en caso de éxito
 *                  -1 en caso de error
 *                  La condición de error se indica en la variable global errno
 */
off_t _lseek(int fd, off_t offset, int whence){
	bsp_dev_t * dev = get_dev (fd);     /* Buscamos el dispositivo en la tabla de dispositivos del BSP */

    /* Si el dispositivo existe y tiene implementada la función lseek, devolvemos su salida */
    if (dev != NULL && dev->lseek != NULL){
        return dev->lseek(dev->id, offset, whence);
    }

    /* En otro caso */
    else{
        return 0;           /* Devolvemos 0, es decir, el inicio del fichero */
    }
}

/*****************************************************************************/

/**
 * Obtención de información de un dispositivo/fichero
 * @param fd    Descriptor de fichero/dispositivo
 * @param buf   Estructura para almacenar dicha información
 * @return      0 en caso de éxito
 *              -1 en caso de error
 *              La condición de error se indica en la variable global errno
 */
int _fstat(int fd, struct stat *buf){
    bsp_dev_t * dev = get_dev (fd);     /* Buscamos el dispositivo en la tabla de dispositivos del BSP */

    /* Si el dispositivo existe y tiene implementada la función fstat, devolvemos su salida */
    if (dev != NULL && dev->fstat != NULL){
        return dev->fstat(dev->id, buf);
    }

    /* En otro caso */
    else{
        buf->st_mode = S_IFCHR;     /* Está abierto sobre un dispositivo de caracteres */
        return 0;                   /* Devolvemos 0, es decir, no hay errores */
    }
}

/*****************************************************************************/

/**
 * Chequea si el descriptor de fichero corresponde con una terminal
 * @param fd    Descriptor de fichero/dispositivo
 * @return      1 si el descriptor corresponde a una terminal
 *              0 en otro caso.
 *              La condición de error se indica en la variable global errno
 */
int _isatty (int fd){
	bsp_dev_t * dev = get_dev (fd);     /* Buscamos el dispositivo en la tabla de dispositivos del BSP */

    /* Si el dispositivo existe y tiene implementada la función isatty, devolvemos su salida */
    if (dev != NULL && dev->isatty != NULL){
        return dev->isatty(dev->id);
    }

    /* En otro caso */
    else{
        return 1;           /* Devolvemos 1, es una terminal 100% REAL, NO FAKE */
    }
}

/*****************************************************************************/
