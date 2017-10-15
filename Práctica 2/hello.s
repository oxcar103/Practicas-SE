@
@ Sistemas Empotrados
@ El "hola mundo" en la Redwire EconoTAG
@

@
@ Constantes
@

        @ Registro de control de dirección del GPIO00-GPIO31 y GPIO32-GPIO63
        .set GPIO_PAD_DIR0,     0x80000000
        .set GPIO_PAD_DIR1,     0x80000004

        @ Registro de datos del GPIO00-GPIO31 y GPIO32-GPIO63
        .set GPIO_DATA_SET0,    0x80000008
        .set GPIO_DATA_SET1,    0x8000000c

        @ Registro de activación de bits del GPIO00-GPIO31 y GPIO32-GPIO63
        .set GPIO_DATA_SET0,    0x80000048
        .set GPIO_DATA_SET1,    0x8000004c

        @ Registro de limpieza de bits del GPIO00-GPIO31 y GPIO32-GPIO63
        .set GPIO_DATA_RESET0,  0x80000050
        .set GPIO_DATA_RESET1,  0x80000054


        @ La salida del botón S2 es GPIO23 y el del botón S3 es GPIO22
        .set BUTTON_S2_MASK,    (1 << 23)
        .set BUTTON_S3_MASK,    (1 << 22)

        @ El led rojo está en el GPIO 44 y el verde en el GPIO 45
        .set LED_RED_MASK,      (1 << (44-32))
        .set LED_GREEN_MASK,    (1 << (45-32))

        @ Retardo para el parpadeo
        .set DELAY,             0x00080000

@
@ Punto de entrada
@

        .code 32
        .text
        .global _start
        .type   _start, %function

_start:
        bl      gpio_init

loop:
        @ Encendemos el led
        str     r5, [r6]

        @ Pausa corta
        ldr     r0, =DELAY
        bl      pause

        @ Apagamos el led
        str     r5, [r7]

        @ Pausa corta
        ldr     r0, =DELAY
        bl      pause

        @ Bucle infinito
        b       loop
        
@
@ Función que produce un retardo
@ r0: iteraciones del retardo
@
        .type   pause, %function
pause:
        subs    r0, r0, #1
        bne     pause
        mov     pc, lr

@
@ Función que inicializa leds y botones
@
        .type   gpio_init, %function
gpio_init:
        @ No necesitamos configurar GPIO22, GPIO23, GPIO26 y GPIO26 ya que usaremos sus valores por defecto
        @ Dirección del registro GPIO_DATA_SET0
        ldr     r6, =GPIO_DATA_SET0

        @ Establecemos a 1 la salida de los GPIO22 y GPIO23
        ldr     r5, =(BUTTON_S2_MASK | BUTTON_S3_MASK)
        str     r5, [r6]

        @ Configuramos GPIO44 y GPIO45 como salida estableciendo el led rojo como por defecto
        ldr     r4, =GPIO_PAD_DIR1
        ldr     r5, =(LED_RED_MASK | LED_GREEN_MASK)
        str     r5, [r4]
        ldr     r5, =LED_RED_MASK

        @ Direcciones de los registros GPIO_DATA_SET1 y GPIO_DATA_RESET1
        ldr     r6, =GPIO_DATA_SET1
        ldr     r7, =GPIO_DATA_RESET1

        @ Retornamos a donde se invocó la función
        mov     pc, lr
