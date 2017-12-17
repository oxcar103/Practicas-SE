/*****************************************************************************/
/*                                                                           */
/* Sistemas Empotrados                                                       */
/* Testeo de la UART en la Redwire EconoTAG en C                             */
/*                                                                           */
/*****************************************************************************/

#include <stdint.h>
#include <string.h>
#include "system.h"

/*
 * Constantes relativas a la plataforma
 */
#define LED_RED         gpio_pin_44
#define LED_GREEN       gpio_pin_45
#define UART_ID         UART1_ID
 
/*
 * Constantes relativas a la aplicacion
 */
uint32_t const delay = 0x20000;

/*
 * Parpadeo de los leds: 0 no, 1 sí
 */
uint32_t blink_red;
uint32_t blink_green;

/*****************************************************************************/

/*
 * Inicialización de los pines de E/S
 */
void gpio_init(void){
    /* Configuramos GPIO 44 y GPIO 45 para que sean de salida */
    gpio_set_pin_dir_output(LED_RED);
    gpio_set_pin_dir_output(LED_GREEN);
}

/*****************************************************************************/

/*
 * Enciende el led indicado
 * @param led   Pin del led a encender
 */
void leds_on (uint32_t led){
    /* Encendemos el led indicado */
    gpio_set_pin(led);
}

/*****************************************************************************/

/*
 * Apaga el led indicado
 * @param led   Pin del led a apagar
 */
void leds_off (uint32_t led){
    /* Apagamos el led indicado */
    gpio_clear_pin(led);
}

/*****************************************************************************/

/*
 * Retardo para el parpedeo
 */
void pause(void){
    uint32_t i;
    for (i=0 ; i<delay ; i++);
}

/*****************************************************************************/

/*
 * Callback de recepción
 */
void my_rx_callback(void){
    char c, * msg;
    size_t n_msg;

    /* Leemos un caracter */
    uart_receive(UART_ID, &c, 1);

    uart_send(UART_ID,"\n\r", 2);               /* Salto de línea, muy necesario */

    /* Si es 'r': */
    if(c == 'r'){
        /* Cambiamos el estado del led rojo */
        blink_red = (blink_red == 0) ? 1 : 0;

        /* Imprimimos un mensaje informando */
        msg="Led rojo modificado\n\r";          /* Guardamos el mensaje */
        n_msg = strlen(msg);                    /* Calculamos su tamaño */
        uart_send(UART_ID, msg, n_msg);         /* Lo mandamos imprimir */
    }

    /* Si es 'g': */
    else if( c == 'g'){
        /* Cambiamos el estado del led verde */
        blink_green = (blink_green == 0) ? 1 : 0;

        /* Imprimimos un mensaje informando */
        msg="Led verde modificado\n\r";         /* Guardamos el mensaje */
        n_msg = strlen(msg);                    /* Calculamos su tamaño */
        uart_send(UART_ID, msg, n_msg);         /* Lo mandamos imprimir */
    }

    /* Si no es 'r' ni 'g': */
    else{
        /* Imprimimos un mensaje de error */
        msg="Carácter incorrecto: Envía 'r' para modificar el led rojo y 'g' para el verde\n\r";    /* Guardamos el mensaje*/
        n_msg = strlen(msg);                    /* Calculamos su tamaño */
        uart_send(UART_ID, msg, n_msg);         /* Lo mandamos imprimir */
    }
}

/*****************************************************************************/

/*
 * Programa principal
 */
int main (){
    char * msg;
    size_t n_msg;

    /* Parpadeo de los leds activado al principio */
    blink_red = 1;         /* Led rojo */
    blink_green = 1;       /* Led verde */

    /* Inicialización del GPIO */
    gpio_init();

    /* Establecemos nuestra función de callback de recepción */
    uart_set_receive_callback (UART_ID, my_rx_callback);

    /* Imprimimos las instrucciones de uso */
    msg="Envía 'r' para modificar el led rojo y 'g' para el verde\n\r";     /* Guardamos el mensaje*/
    n_msg = strlen(msg);                        /* Calculamos su tamaño */
    uart_send(UART_ID, msg, n_msg);             /* Lo mandamos imprimir */

    /* Bucle infinito de parpadeo de los leds si sus estados lo permiten */
    while (1){
        if(blink_red == 1){
            leds_on(LED_RED);
        }
        if(blink_green == 1){
            leds_on(LED_GREEN);
        }

        pause();

        leds_off(LED_RED);
        leds_off(LED_GREEN);

        pause();
    }

    return 0;
}

/*****************************************************************************/
