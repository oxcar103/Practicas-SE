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
 * Estado de los leds: 0 apagado, 1 encendido
 */
uint32_t led_state[2];
 
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
 * Programa principal
 */
int main (){
    int change = 0, the_led;
    char c;

    gpio_init();

    /* Leds apagados al principio */
    led_state[0] = 0;
    led_state[1] = 0;

    send_str(UART_ID, "Envía 'r' para modificar el led rojo y 'g' para el verde\n\r");

    while (1){
        change=0;
        c = uart_receive_byte(UART_ID);
        send_str(UART_ID, "\n\r");

        if (c == 'r'){
            the_led=LED_RED;
            send_str(UART_ID, "Led rojo ");
            change=1;
        }
        else if (c == 'g'){
            the_led=LED_GREEN;
            send_str(UART_ID, "Led verde ");
            change=1;
        }
        else{
            send_str(UART_ID, "Carácter incorrecto: Envía 'r' para modificar el led rojo y 'g' para el verde\n\r");
        }


        if(change != 0){
            if(led_state[the_led] == 0){
                leds_on(the_led);
                send_str(UART_ID, "encendido\n\r");
            }
            else{
                leds_off(the_led);
                send_str(UART_ID, "apagado\n\r");
            }

            led_state[the_led] ^= 1;
        }
    }

    return 0;
}

/*****************************************************************************/

