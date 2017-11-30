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
    /* Configuramos el GPIO44 para que sea de salida */
    *reg_gpio_pad_dir1 = led_red_mask | led_green_mask;
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

