/*
 * Sistemas operativos empotrados
 * Buffer circular
 */

#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <stdint.h>

/*****************************************************************************/

/**
 * Estructura para gestionar un buffer circular
 */
typedef struct{
    uint8_t *data;
    uint32_t size;
    uint32_t start;
    uint32_t end;
    uint32_t count;
} circular_buffer_t;

/*****************************************************************************/

/**
 * Inicializa un buffer circular dado un puntero a una zona de memoria y su tamaño
 * @param cb    Puntero a la estructura de gestión del buffer circular
 * @param addr  Puntero a la zona de memoria que se gestionará como un buffer circular
 * @param size  Tamaño en bytes del buffer
 */
void circular_buffer_init (volatile circular_buffer_t *cb, uint8_t *addr, uint32_t size);

/*****************************************************************************/

/**
 * Comprueba si el buffer está lleno
 * @param cb    Buffer circular
 * @return      1 si el buffer está lleno
 *              0 si no lo está
 */
inline uint32_t circular_buffer_is_full (volatile circular_buffer_t *cb);

/*****************************************************************************/

/**
 * Comprueba si el buffer está vacío
 * @param cb    Buffer circular
 * @return      1 si el buffer está vacío
 *              0 si no lo está
 */
inline uint32_t circular_buffer_is_empty (volatile circular_buffer_t *cb);

/*****************************************************************************/

/**
 * Escribe un byte en un buffer circular
 * @param cb    Buffer circular
 * @param byte  Byte a escribir
 * @return      Byte escrito en caso de éxito
 *              -1 en caso de error
 */
int32_t circular_buffer_write (volatile circular_buffer_t *cb, uint8_t byte);

/*****************************************************************************/

/**
 * Lee un byte en un buffer circular
 * @param cb    Buffer circular
 * @return      Byte leído en caso de éxito
 *              -1 en caso de error
 */
int32_t circular_buffer_read (volatile circular_buffer_t *cb);

/*****************************************************************************/

#endif /* __CIRCULAR_BUFFER_H__ */
