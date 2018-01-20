/*****************************************************************************/
/*                                                                           */
/* Sistemas Empotrados                                                       */
/* Testeo de botones en la Redwire EconoTAG en C                             */
/*                                                                           */
/*****************************************************************************/

#include <stdint.h>
#include "system.h"

/*
 * Constantes relativas a la plataforma
 */

#define LED_RED         gpio_pin_44
#define LED_GREEN       gpio_pin_45
#define BUTTON_S2_OUT   gpio_pin_23
#define BUTTON_S3_OUT   gpio_pin_22
#define BUTTON_S2_IN    gpio_pin_27
#define BUTTON_S3_IN    gpio_pin_26
 
/*
 * Constantes relativas a la aplicación
 */
uint32_t const delay = 0x20000;

/*
 * Variables globales: máscaras del led que se hará parpadear y del botón que se comprobará
 */
uint32_t the_led;
uint32_t the_button;
 
/*****************************************************************************/

/*
 * Inicialización de los pines de E/S
 */
void gpio_init(void){
    /* Configuramos GPIO 44 y GPIO 45 para que sean de salida */
    gpio_set_pin_dir_output(LED_RED);
    gpio_set_pin_dir_output(LED_GREEN);

    /* No es necesario configurar los GPIO 22, GPIO 23, GPIO 26 y GPIO 27 porque usaremos su valor por defecto */

    /* Fijamos a 1 la salida de los botones */
    gpio_set_pin(BUTTON_S2_OUT);
    gpio_set_pin(BUTTON_S3_OUT);
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
 * Comprueba si se pulsó el botón esperado, y cambia led y botón en caso afirmativo
 */
void test_button(void){
    uint32_t value;
    gpio_get_pin(the_button, &value);

    if(value != 0){
        the_led = (the_led == LED_RED) ? LED_GREEN : LED_RED;
        the_button = (the_button == BUTTON_S3_IN) ? BUTTON_S2_IN : BUTTON_S3_IN;
    }
}

/*****************************************************************************/

/*
 * Programa principal
 */
int main (){
    gpio_init();

    the_led = LED_RED;
    the_button = BUTTON_S3_IN;
    while (1){
        test_button();
        leds_on(the_led);
        pause();

        leds_off(the_led);
        test_button();
        pause();
    }

    return 0;
}

/*****************************************************************************/

