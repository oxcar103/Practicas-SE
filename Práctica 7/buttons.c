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
 * Constantes relativas a la aplicacion
 */
uint32_t const delay = 0x20000;
 
/*****************************************************************************/

/*
 * Inicialización de los pines de E/S
 */
void gpio_init(void){
    /* Configuramos GPIO 44 y GPIO 45 para que sean de salida */
    gpio_set_pin_dir_output(LED_RED);
    gpio_set_pin_dir_output(LED_GREEN);

    /* No es necesario configurar los GPIO22, GPIO 23, GPIO 26 y GPIO 27 porque usaremos su valor por defecto */

    /* Fijamos a 1 la salida de los botones */
    gpio_set_pin(BUTTON_S2_OUT);
    gpio_set_pin(BUTTON_S3_OUT);
}

/*****************************************************************************/

/*
 * Enciende los leds indicados en la máscara
 * @param mask  Máscara para seleccionar leds
 */
void leds_on (uint32_t mask){
    /* Encendemos los leds indicados en la máscara */
    *reg_gpio_data_set1 = mask;
}

/*****************************************************************************/

/*
 * Apaga los leds indicados en la máscara
 * @param mask  Máscara para seleccionar leds
 */
void leds_off (uint32_t mask){
    /* Apagamos los leds indicados en la máscara */
    *reg_gpio_data_reset1 = mask;
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
 * Manejador de interrupciones asm
 */
void my_asm_handler(void){
    itc_unforce_interrupt(itc_src_asm);
    leds_on (led_green_mask);
}

/*****************************************************************************/

/*
 * Variables globales: máscaras del led que se hará parpadear y del botón que se comprobará
 */
uint32_t the_led;
uint32_t the_button;

/*
 * Programa principal
 */
int main (){
    // Ejecutamos gpio_init() dentro de una sección crítica
    itc_disable_ints();
    gpio_init();
    itc_restore_ints();

    // Asignamos el nuevo manejador a la interrupción asm
    itc_set_handler (itc_src_asm, my_asm_handler);

    // Habilitamos la interrupción asm
    itc_enable_interrupt (itc_src_asm);

    the_led = led_red_mask;

    // Fuerza la interrupción asm
    itc_force_interrupt(itc_src_asm);

    while (1){
        leds_on(the_led);
        pause();

        leds_off(the_led);
        pause();
    }

    return 0;
}

/*****************************************************************************/

