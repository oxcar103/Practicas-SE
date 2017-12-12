/*
 * Sistemas operativos empotrados
 * Buffer circular
 */

#include "circular_buffer.h"

/*****************************************************************************/

/**
 * Inicializa un buffer circular dado un puntero a una zona de memoria y su tamaño
 * @param cb    Puntero a la estructura de gestión del buffer circular
 * @param addr  Puntero a la zona de memoria que se gestionará como un buffer circular
 * @param size  Tamaño en bytes del buffer
 */
void circular_buffer_init (volatile circular_buffer_t *cb, uint8_t *addr, uint32_t size){
    cb->data = addr;
    cb->size = size;
    cb->start = 0;
    cb->end = 0;
    cb->count = 0;
}

/*****************************************************************************/

/**
 * Retorna 1 si el buffer está lleno
 * @param cb    Buffer circular
 */
inline uint32_t circular_buffer_is_full (volatile circular_buffer_t *cb){
    return cb->count == cb->size;
}

/*****************************************************************************/

/**
 * Retorna 1 si el buffer está vacío
 * @param cb    Buffer circular
 */
inline uint32_t circular_buffer_is_empty (volatile circular_buffer_t *cb){
    return cb->count == 0;
}

/*****************************************************************************/

/**
 * Escribe un byte en un buffer circular
 * @param cb    Buffer circular
 * @param byte  Byte a escribir
 * @return      El byte (tras un casting a int32_t) en caso de éxito
 *              -1 en caso de error
 */
int32_t circular_buffer_write (volatile circular_buffer_t *cb, uint8_t byte){
    /* Escribimos en el buffer sólo si hay espacio */
    if (circular_buffer_is_full(cb)){
        return -1;
    }
    else{
        cb->data[cb->end] = byte;
        cb->count++;

        cb->end++;

        if (cb->end == cb->size){
            cb->end = 0;
        }

        return byte;
    }
}

/*****************************************************************************/

/**
 * Lee un byte en un buffer circular
 * @param cb    Buffer circular
 * @return      El byte (tras un casting a int32_t) en caso de éxito
 *              -1 en caso de error
 */
int32_t circular_buffer_read (volatile circular_buffer_t *cb){
    int32_t byte;

    /* Leemos del buffer sólo si hay algo */
    if (circular_buffer_is_empty(cb)){
        return -1;
    }
    else{
        byte = cb->data[cb->start];
        cb->count--;

        cb->start++;

        if (cb->start == cb->size){
            cb->start = 0;
        }

        return byte;
    }
}

/*****************************************************************************/
