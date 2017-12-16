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
    /* Parpadear los leds: 0 no, 1 sí */
    uint32_t blink_led[2];

    /* Inicialización del GPIO */
    gpio_init();

    /* Parpadeo de los leds activado al principio */
    blink_led[0] = 1;       /* Led rojo */
    blink_led[1] = 1;       /* Led verde */

    while (1){
        if(blink_led[0]=1){
            leds_on(LED_RED);
        }
        if(blink_led[1]=1){
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

