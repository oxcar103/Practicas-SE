/*
 * Sistemas operativos empotrados
 * Driver de las uart
 */

#include <fcntl.h>
#include <errno.h>
#include "system.h"
#include "circular_buffer.h"

/*****************************************************************************/

/**
 * Acceso estructurado a los registros de control de las uart del MC1322x
 */

typedef struct{
    union{
        struct{
            uint32_t TxE                :1;
            uint32_t RxE                :1;
            uint32_t PEN                :1;
            uint32_t EP                 :1;
            uint32_t ST2                :1;
            uint32_t SB                 :1;
            uint32_t conTx              :1;
            uint32_t Tx_oen_b           :1;
            uint32_t                    :2;
            uint32_t xTIM               :1;
            uint32_t FCp                :1;
            uint32_t FCe                :1;
            uint32_t mTxr               :1;
            uint32_t mRxr               :1;
            uint32_t TST                :1;
        };
        uint32_t CON;
    };

    union{
        struct{
            uint32_t SE                 :1;
            uint32_t PE                 :1;
            uint32_t FE                 :1;
            uint32_t TOE                :1;
            uint32_t ROE                :1;
            uint32_t RUE                :1;
            uint32_t RxRdy              :1;
            uint32_t TxRdy              :1;
        };
        uint32_t STAT;
    };

    union{
        uint8_t Rx_data;
        uint8_t Tx_data;
        uint32_t DATA;
    };

    union{
        uint32_t RxLevel                :5;
        uint32_t Rx_fifo_addr_diff      :6;
        uint32_t RxCON;
    };

    union{
        uint32_t TxLevel                :5;
        uint32_t Tx_fifo_addr_diff      :6;
        uint32_t TxCON;
    };

    union{
        uint32_t CTS_Level              :5;
        uint32_t CTS;
    };

    union{
        struct{
            uint16_t BRMOD;
            uint16_t BRINC;
        };
        uint32_t BR;
    };
} uart_regs_t;

/*****************************************************************************/

/**
 * Acceso estructurado a los pines de las uart del MC1322x
 */
typedef struct{
    gpio_pin_t tx,rx,cts,rts;
} uart_pins_t;

/*****************************************************************************/

/**
 * Definición de las UARTS
 */
static volatile uart_regs_t* const uart_regs[uart_max] = {UART1_BASE, UART2_BASE};

static const uart_pins_t uart_pins[uart_max] = {
        {gpio_pin_14, gpio_pin_15, gpio_pin_16, gpio_pin_17},
        {gpio_pin_18, gpio_pin_19, gpio_pin_20, gpio_pin_21} };

static void uart_1_isr (void);
static void uart_2_isr (void);
static const itc_handler_t uart_irq_handlers[uart_max] = {uart_1_isr, uart_2_isr};

/*****************************************************************************/

/**
 * Tamaño de los buffers circulares
 */
#define __UART_BUFFER_SIZE__	256

static volatile uint8_t uart_rx_buffers[uart_max][__UART_BUFFER_SIZE__];
static volatile uint8_t uart_tx_buffers[uart_max][__UART_BUFFER_SIZE__];

static volatile circular_buffer_t uart_circular_rx_buffers[uart_max];
static volatile circular_buffer_t uart_circular_tx_buffers[uart_max];


/*****************************************************************************/

/**
 * Gestión de las callbacks
 */
typedef struct{
    uart_callback_t tx_callback;
    uart_callback_t rx_callback;
} uart_callbacks_t;

static volatile uart_callbacks_t uart_callbacks[uart_max];

/*****************************************************************************/

/**
 * Inicializa una uart
 * @param uart  Identificador de la uart
 * @param br    Baudrate
 * @param name  Nombre del dispositivo
 * @return      0 en caso de éxito
 *              -1 en caso de error
 *              La condición de error se indica en la variable global errno
 */
int32_t uart_init (uart_id_t uart, uint32_t br, const char *name){
    if(uart >= uart_max){
        errno = ENODEV;

        return -1;
    }

    if(name = NULL){
        errno = EFAULT;

        return -1;
    }

    /* Calculamos mod e inc asumiendo un oversampling de 8x y un Peripheral_Bus_Blk = 24Mhz */
    uint16_t mod = 9999;
    uint16_t inc = br * mod / (CPU_FREQ >> 4) - 1;

    /* Fijamos los parámetros por defecto y deshabilitamos la uart */
    /* La uart debe estar deshabilitada para fijar la frecuencia */
    uart_regs[uart]->CON = (1 << 13) | (1 << 14);

    /* Fijamos la frecuencia */
    //uart_regs[uart]->BRINC = inc;
    //uart_regs[uart]->BRMOD = mod;
    uart_regs[uart]->BR = (inc << 16) | mod;

    /* Habilitamos la uart */
    /* En el MC1322x hay que habilitar el periférico antes fijar el modo de funcionamiento de sus pines */
    uart_regs[uart]->TxE=1;
    uart_regs[uart]->RxE=1;

    /* Cambiamos el modo de funcionamiento de los pines */
    gpio_set_pin_func (uart_pins[uart].tx, gpio_func_alternate_1);
    gpio_set_pin_func (uart_pins[uart].rx, gpio_func_alternate_1);
    gpio_set_pin_func (uart_pins[uart].cts, gpio_func_alternate_1);
    gpio_set_pin_func (uart_pins[uart].rts, gpio_func_alternate_1);

    /* Fijamos TX y CTS como salidas y RX y RTS como entradas */
    gpio_set_pin_dir_output (uart_pins[uart].tx);
    gpio_set_pin_dir_output (uart_pins[uart].cts);
    gpio_set_pin_dir_input (uart_pins[uart].rx);
    gpio_set_pin_dir_input (uart_pins[uart].rts);

    /* Inicializamos los bufferes circulares */
    circular_buffer_init(&uart_circular_rx_buffers[uart], (uint8_t *) uart_rx_buffers[uart], sizeof(uart_rx_buffers[uart]));
    circular_buffer_init(&uart_circular_tx_buffers[uart], (uint8_t *) uart_tx_buffers[uart], sizeof(uart_tx_buffers[uart]));

    /* Fijamos los valores de activación de las interrupciones */
    uart_regs[uart]->TxLevel=31;            /* Cuando queden 31 huecos libres en la cola de transmisión */
    uart_regs[uart]->RxLevel=1;             /* Cuando se haya llenado 1 hueco en la cola de recepción */

    /* Habilitamos las interrupciones de la uart en el ITC */
    itc_set_priority (itc_src_uart1 + uart, itc_priority_normal);
    itc_set_handler (itc_src_uart1 + uart, uart_irq_handlers[uart]);
    itc_enable_interrupt (itc_src_uart1 + uart);

    /* Inicializamos los callbacks */
    uart_callbacks[uart].tx_callback = NULL;
    uart_callbacks[uart].rx_callback = NULL;
    

    /* Y habilitamos las interrupciones de recepción */
    uart_regs[uart]->MRxR = 0;
    
    return 0;
}

/*****************************************************************************/

/**
 * Transmite un byte por la uart
 * Implementación del driver de nivel 0
 * La llamada se bloquea hasta que transmite el byte
 * @param uart  Identificador de la uart
 * @param c     El carácter
 */
void uart_send_byte (uart_id_t uart, uint8_t c){
    /* Desactivamos las interrupciones guardando su valor */
    uint32_t old_mTxR = uart_regs[uart]->mTxR;
    uart_regs[uart]->mTxR = 1;

    /* Vaciamos primero el buffer circular */
    while (!circular_buffer_is_empty (&uart_circular_tx_buffers[uart])){
        if(uart_regs[uart]->Tx_fifo_addr_diff > 0){
            uart_regs[uart]->Tx_data = circular_buffer_read (&uart_circular_tx_buffers[uart]);
        }
    }

    /* Esperamos a poder transmitir */
    while (uart_regs[uart]->Tx_fifo_addr_diff == 0);
    
    /* Escribimos el carácter en la cola HW de la uart */
    uart_regs[uart]->Tx_data = c;

    /* Restauramos el estado de las interrupciones */
    uart_regs[uart]->mTxR = old_mTxR;

}

/*****************************************************************************/

/**
 * Recibe un byte por la uart
 * Implementación del driver de nivel 0
 * La llamada se bloquea hasta que recibe el byte
 * @param uart  Identificador de la uart
 * @return      El byte recibido
 */
uint8_t uart_receive_byte (uart_id_t uart){
    /* Esperamos a que haya datos que leer */
    while (uart_regs[uart]->Rx_fifo_addr_diff == 0);
    
    /* Leemos el byte */
    return uart_regs[uart]->Rx_data;
}

/*****************************************************************************/

/**
 * Transmisión de bytes
 * Implementación del driver de nivel 1
 * La llamada es no bloqueante y se realiza mediante interrupciones
 * @param uart  Identificador de la uart
 * @param buf   Buffer con los caracteres
 * @param count Número de caracteres a escribir
 * @return      Número de bytes almacenados en caso de éxito
 *              -1 en caso de error
 *              La condición de error se indica en la variable global errno
 */
ssize_t uart_send (uint32_t uart, char *buf, size_t count){
    ssize_t copied_bytes = 0;

    if(uart >= uart_max){
        errno = ENODEV;

        return -1;
    }

    if(buf = NULL || count < 0){
        errno = EFAULT;

        return -1;
    }

    /* Desactivamos las interrupciones */
    uart_regs[uart]->mTxR = 1;

    /* Copiamos los datos que podamos al buffer circular */
    while(count > 0 && !circular_buffer_is_full (&uart_circular_tx_buffers[uart])){
        circular_buffer_write (&uart_circular_tx_buffers[uart], *buf++);

        copied_bytes++;
        count--;
    }

    /* Activamos las interrupciones */
    uart_regs[uart]->mTxR = 0;

    return copied_bytes;
}

/*****************************************************************************/

/**
 * Recepción de bytes
 * Implementación del driver de nivel 1
 * La llamada es no bloqueante y se realiza mediante interrupciones
 * @param uart  Identificador de la uart
 * @param buf   Buffer para almacenar los bytes
 * @param count Número de bytes a leer
 * @return      Número de bytes leídos en caso de éxito
 *              -1 en caso de error
 *              La condición de error se indica en la variable global errno
 */
ssize_t uart_receive (uint32_t uart, char *buf, size_t count){
    ssize_t copied_bytes = 0;

    if(uart >= uart_max){
        errno = ENODEV;

        return -1;
    }

    if(buf = NULL || count < 0){
        errno = EFAULT;

        return -1;
    }

    /* Desactivamos las interrupciones */
    uart_regs[uart]->mRxR = 1;

    /* Copiamos los datos que podamos del buffer circular */
    while(count > 0 && !circular_buffer_is_empty (&uart_circular_rx_buffers[uart])){
        circular_buffer_read (&uart_circular_rx_buffers[uart], *buf++);

        copied_bytes++;
        count--;
    }

    /* Activamos las interrupciones */
    uart_regs[uart]->mRxR = 0;

    return copied_bytes;
}

/*****************************************************************************/

/**
 * Fija la función callback de transmisión de una uart
 * @param uart  Identificador de la uart
 * @param func  Función callback. NULL para anular una selección anterior
 * @return      0 en caso de éxito
 *              -1 en caso de error
 *              La condición de error se indica en la variable global errno
 */
int32_t uart_set_send_callback (uart_id_t uart, uart_callback_t func){
    if(uart >= uart_max){
        errno = ENODEV;

        return -1;
    }

    uart_callbacks[uart]->tx_callback = func;

    return 0;
}

/*****************************************************************************/

/**
 * Fija la función callback de recepción de una uart
 * @param uart  Identificador de la uart
 * @param func  Función callback. NULL para anular una selección anterior
 * @return      0 en caso de éxito
 *              -1 en caso de error
 *              La condición de error se indica en la variable global errno
 */
int32_t uart_set_receive_callback (uart_id_t uart, uart_callback_t func){
    if(uart >= uart_max){
        errno = ENODEV;

        return -1;
    }

    uart_callbacks[uart]->rx_callback = func;

    return 0;
}

/*****************************************************************************/

/**
 * Manejador genérico de interrupciones para las uart.
 * Cada isr llamará a este manejador indicando la uart de la interrupción.
 * Declarado inline para reducir la latencia de la isr.
 * @param uart  Identificador de la uart
 */
static inline void uart_isr (uart_id_t uart){
    uint32_t status = uart_regs[uart]->stat;

    /* Interrupción de Transmisión (quiere más datos) */
    if(uart_regs[uart]->TxRdy){
        /* Le pasamos desde el buffer circular tanto como podemos */
        while(uart_regs[uart]->Tx_fifo_addr_diff > 0 && !circular_buffer_is_empty (&uart_circular_tx_buffers[uart])){
            uart_regs[uart]->Tx_data = circular_buffer_read (&uart_circular_tx_buffers[uart]);
        }

        /* Si hay definida una función callback... */
        if(uart_callbacks[uart]->tx_callback){
            /* ... la llamamos */
            uart_callbacks[uart]->tx_callback;
        }

        /* Si el buffer circular está vacío... */
        if(circular_buffer_is_empty (&uart_circular_tx_buffers[uart])){
            /* ... desactivamos las interrupciones */
            uart_regs[uart]->mTxR = 1;
        }
    }

    /* Interrupción de Recepción (hay nuevos datos) */
    if(uart_regs[uart]->RxRdy){
        /* Le pasamos al buffer circular tanto como podemos */
        while(uart_regs[uart]->Rx_fifo_addr_diff > 0 && !circular_buffer_is_full (&uart_circular_rx_buffers[uart])){
            circular_buffer_write (&uart_circular_rx_buffers[uart], uart_regs[uart]->Rx_data);
        }

        /* Si hay definida una función callback... */
        if(uart_callbacks[uart]->rx_callback){
            /* ... la llamamos */
            uart_callbacks[uart]->rx_callback;
        }

        /* Si el buffer circular está lleno... */
        if(circular_buffer_is_full (&uart_circular_rx_buffers[uart])){
            /* ... desactivamos las interrupciones */
            uart_regs[uart]->mRxR = 1;
        }
    }
}

/*****************************************************************************/

/**
 * Manejador de interrupciones para la uart1
 */
static void uart_1_isr (void){
    uart_isr(uart_1);
}

/*****************************************************************************/

/**
 * Manejador de interrupciones para la uart2
 */
static void uart_2_isr (void){
    uart_isr(uart_2);
}

/*****************************************************************************/
