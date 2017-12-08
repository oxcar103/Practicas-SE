/*****************************************************************************/
/*                                                                           */
/* Sistemas Empotrados                                                       */
/* Testeo de la UART en la Redwire EconoTAG en C                             */
/*                                                                           */
/*****************************************************************************/

#include <stdint.h>
#include "system.h"

/*
 * Constantes relativas a la plataforma
 */
#define LED_RED         gpio_pin_44
#define LED_GREEN       gpio_pin_45
 
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
 * Envía todo el string que se le pase
 * @param str   String a enviar
 */
void send_str (char * str){
    while (*str)
        uart_send_byte(UART1_ID, *str++);
}

/*****************************************************************************/

/*
 * Recibe hasta encontrar un fin de línea
 * @param led   Pin del led a apagar
 */
char * recieve_str (){
    char * str="";
    char c=''; 
    do{
        c=uart_receive_byte (UART1_ID);
        strcat(str, c);
    }while(c != '\n');

    return str;
}

/*****************************************************************************/

/*
 * Programa principal
 */
int main (){
    gpio_init();

    /* Leds apagados al principio */
    led_state[0] = 0;
    led_state[1] = 0;

    while (1){
        
        if(led_state[the_led] == 0){
            leds_on(the_led);
        }
        else{
            leds_off(the_led);
        }

        led_state[the_led] ^= 1;
    }

    return 0;
}

/*****************************************************************************/

