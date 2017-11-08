/*****************************************************************************/
/*                                                                           */
/* Sistemas Empotrados                                                       */
/* El "hola mundo" en la Redwire EconoTAG en C                               */
/*                                                                           */
/*****************************************************************************/

#include <stdint.h>

/*
 * Constantes relativas a la plataforma
 */

/* Direcciones de los registros de control de dirección de los GPIO00-GPIO31 y GPIO32-GPIO63 */
volatile uint32_t * const reg_gpio_pad_dir0     = (uint32_t *) 0x80000000;
volatile uint32_t * const reg_gpio_pad_dir1     = (uint32_t *) 0x80000004;

/* Direcciones de los registros de datos de los GPIO00-GPIO31 y GPIO32-GPIO63 */
volatile uint32_t * const reg_gpio_data0        = (uint32_t *) 0x80000008;
volatile uint32_t * const reg_gpio_data1        = (uint32_t *) 0x8000000c;

/* Direcciones de los registros de activación de bits de los GPIO00-GPIO31 y GPIO32-GPIO63 */
volatile uint32_t * const reg_gpio_data_set0    = (uint32_t *) 0x80000048;
volatile uint32_t * const reg_gpio_data_set1    = (uint32_t *) 0x8000004c;

/* Direcciones de los registros de limpieza de bits de los GPIO00-GPIO31 y GPIO32-GPIO63 */
volatile uint32_t * const reg_gpio_data_reset0  = (uint32_t *) 0x80000050;
volatile uint32_t * const reg_gpio_data_reset1  = (uint32_t *) 0x80000054;


/* La salida del botón S2 está en el GPIO 23 y la del botón S3 en el GPIO 22 */
uint32_t const button_s2_out =  (1 << 23);
uint32_t const button_s3_out =  (1 << 22);

/* La entrada del botón S2 está en el GPIO 27 y la del botón S3 en el GPIO 26 */
uint32_t const button_s2_in =   (1 << 27);
uint32_t const button_s3_in =   (1 << 26);

/* El led rojo está en el GPIO 44 y el verde en el GPIO 45 (bits 12 y 13 de los registros GPIO_X_1) */
uint32_t const led_red_mask =   (1 << (44-32));
uint32_t const led_green_mask = (1 << (45-32));

/*
 * Constantes relativas a la aplicacion
 */
uint32_t const delay = 0x20000;
 
/*****************************************************************************/

/*
 * Inicialización de los pines de E/S
 */
void gpio_init(void)
{
    /* Configuramos el GPIO44 para que sea de salida */
    *reg_gpio_pad_dir1 = led_red_mask;
}

/*****************************************************************************/

/*
 * Enciende los leds indicados en la máscara
 * @param mask Máscara para seleccionar leds
 */
void leds_on (uint32_t mask)
{
    /* Encendemos los leds indicados en la máscara */
    *reg_gpio_data_set1 = mask;
}

/*****************************************************************************/

/*
 * Apaga los leds indicados en la máscara
 * @param mask Máscara para seleccionar leds
 */
void leds_off (uint32_t mask)
{
    /* Apagamos los leds indicados en la máscara */
    *reg_gpio_data_reset1 = mask;
}

/*****************************************************************************/

/*
 * Retardo para el parpedeo
 */
void pause(void)
{
    uint32_t i;
    for (i=0 ; i<delay ; i++);
}

/*****************************************************************************/

/*
 * Máscara del led que se hará parpadear
 */
uint32_t the_led;

/*
 * Programa principal
 */
int main ()
{
    gpio_init();

    the_led = led_red_mask;

    while (1)
    {
        leds_on(the_led);
        pause();

        leds_off(the_led);
        pause();
    }

    return 0;
}

/*****************************************************************************/

