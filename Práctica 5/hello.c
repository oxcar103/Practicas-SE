/*****************************************************************************/
/*                                                                           */
/* Sistemas Empotrados                                                       */
/* El "hola mundo" en la Redwire EconoTAG en C                               */
/*                                                                           */
/*****************************************************************************/

#include <stdint.h>
#include "system.h"

/*
 * Constantes relativas a la plataforma
 */

/* Dirección del registro de control de dirección del GPIO32-GPIO63 */
volatile uint32_t * const reg_gpio_pad_dir1     = (uint32_t *) 0x80000004;

/* Dirección del registro de activación de bits del GPIO32-GPIO63 */
volatile uint32_t * const reg_gpio_data_set1    = (uint32_t *) 0x8000004c;

/* Dirección del registro de limpieza de bits del GPIO32-GPIO63 */
volatile uint32_t * const reg_gpio_data_reset1  = (uint32_t *) 0x80000054;

/* El led rojo está en el GPIO 44 (el bit 12 de los registros GPIO_X_1) */
uint32_t const led_red_mask =   (1 << (44-32));

/* El led verde está en el GPIO 45 (el bit 13 de los registros GPIO_X_1) */
uint32_t const led_green_mask = (1 << (45-32));

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
 * Manejador de interrupciones UNDEF
 */
__attribute__ ((interrupt ("UNDEF")))
void my_undef_handler(void){
    leds_on (led_green_mask);
}

/*****************************************************************************/

/*
 * Máscara del led que se hará parpadear
 */
uint32_t the_led;

/*
 * Programa principal
 */
int main (){
    // Variable que contendrá el valor actual de (I, F)
    uint32_t bits;

    // Deshabilitamos las interrupciones
    bits = excep_disable_ints();

    // Ejecutamos gpio_init()
    gpio_init();

    // Las habilitamos
    excep_restore_ints(bits);

    // Asignamos el nuevo manejador a la excepción UNDEF
    excep_set_handler (excep_undef, my_undef_handler);

    the_led = led_red_mask;

    // Fuerza el modo UNDEF
    asm(".word 0x26889912\n");

    while (1){
        leds_on(the_led);
        pause();

        leds_off(the_led);
        pause();
    }

    return 0;
}

/*****************************************************************************/

