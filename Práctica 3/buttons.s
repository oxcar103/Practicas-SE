@
@ Sistemas Empotrados
@

@
@ Constantes
@

        @ Registro de control de dirección del GPIO00-GPIO31 y GPIO32-GPIO63
        .set GPIO_PAD_DIR0,     0x80000000
        .set GPIO_PAD_DIR1,     0x80000004

        @ Registro de datos del GPIO00-GPIO31 y GPIO32-GPIO63
        .set GPIO_DATA0,        0x80000008
        .set GPIO_DATA1,        0x8000000c

        @ Registro de activación de bits del GPIO00-GPIO31 y GPIO32-GPIO63
        .set GPIO_DATA_SET0,    0x80000048
        .set GPIO_DATA_SET1,    0x8000004c

        @ Registro de limpieza de bits del GPIO00-GPIO31 y GPIO32-GPIO63
        .set GPIO_DATA_RESET0,  0x80000050
        .set GPIO_DATA_RESET1,  0x80000054

        @ La salida del botón S2 es GPIO23 y el del botón S3 es GPIO22
        @.set BUTTON_S2_OUT,     (1 << 23)
        @.set BUTTON_S3_OUT,     (1 << 22)

        @ La entrada del botón S2 es GPIO27 y el del botón S3 es GPIO26
        @.set BUTTON_S2_IN,      (1 << 27)
        @.set BUTTON_S3_IN,      (1 << 26)

        @ El led rojo está en GPIO44 y el verde en GPIO45
        @.set LED_RED_MASK,      (1 << (44-32))
        @.set LED_GREEN_MASK,    (1 << (45-32))

        @ Retardo para el parpadeo
        @.set DELAY,             0x00080000

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
        @ Comprobamos botones pulsados
        bl      test_buttons

        @ Encendemos el led
        ldr     r4, =LED_ON
        ldr     r5, [r4]
        str     r5, [r8]

        @ Pausa corta
        ldr     r4, =DELAY
        ldr     r0, [r4]
        bl      pause

        @ Apagamos el led
        ldr     r4, =LED_ON
        ldr     r5, [r4]
        str     r5, [r9]

        @ Comprobamos botones pulsados
        bl      test_buttons

        @ Pausa corta
        ldr     r4, =DELAY
        ldr     r0, [r4]
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
        ldr     r7, =GPIO_DATA_SET0

        @ Establecemos a 1 la salida de los GPIO22 y GPIO23
        ldr     r4, =BUTTON_S2_OUT
        ldr     r5, [r4]
        ldr     r4, =BUTTON_S3_OUT
        ldr     r6, [r4]
        orr     r5, r6
        str     r5, [r7]

        @ Configuramos GPIO44 y GPIO45 como salida
        ldr     r7, =GPIO_PAD_DIR1
        ldr     r4, =LED_RED_MASK
        ldr     r5, [r4]
        ldr     r4, =LED_GREEN_MASK
        ldr     r6, [r4]
        orr     r5, r6
        str     r5, [r7]

        @ Fijamos los valores iniciales de las variables que usaremos
        ldr     r4, =BUTTON_S2_IN
        ldr     r5, [r4]
        ldr     r4, =CHECK_BUTTON
        str     r5, [r4]
        
        ldr     r4, =LED_RED_MASK
        ldr     r5, [r4]
        ldr     r4, =LED_ON
        str     r5, [r4]

        ldr     r7, =GPIO_DATA0
        ldr     r8, =GPIO_DATA_SET1
        ldr     r9, =GPIO_DATA_RESET1

        @ Retornamos a donde se invocó la función
        mov     pc, lr

@
@ Función para detectar si un botón se pulsa
@
        .type   test_buttons, %function
test_buttons:
        @ Comprobamos si el botón está pulsado
        ldr     r0, [r7]
        ldr     r4, =CHECK_BUTTON
        ldr     r1, [r4]
        tst     r0, r1

        @ Si lo está, cambiamos el led que debemos encender y el botón que debemos comprobar en otra función
        blne    pressed_button

        @ Retornamos a donde se invocó la función
        mov     pc, lr

@
@ Función para cambiar CHECK_BUTTON y LED_ON dado que se ha pulsado el botón.
@
        .type   pressed_button, %function
pressed_button:
        @ Sumamos ambas máscaras
        ldr     r4, =BUTTON_S2_IN
        ldr     r5, [r4]
        ldr     r4, =BUTTON_S3_IN
        ldr     r6, [r4]
        orr     r5, r6

        @ Permutamos el valor de CHECK_BUTTON
        ldr     r4, =CHECK_BUTTON
        ldr     r6, [r4]
        eor     r5, r6
        str     r5, [r4]

        @ Sumamos ambas máscaras
        ldr     r4, =LED_RED_MASK
        ldr     r5, [r4]
        ldr     r4, =LED_GREEN_MASK
        ldr     r6, [r4]
        orr     r5, r6

        @ Permutamos el valor de LED_ON
        ldr     r4, =LED_ON
        ldr     r6, [r4]
        eor     r5, r6
        str     r5, [r4]

        @ Retornamos a donde se invocó la función
        mov     pc, lr

@ Variables globales
        .data

        @ La salida del botón S2 es GPIO23 y el del botón S3 es GPIO22
        BUTTON_S2_OUT:      .word   (1 << 23)
        BUTTON_S3_OUT:      .word   (1 << 22)

        @ La entrada del botón S2 es GPIO27 y el del botón S3 es GPIO26
        BUTTON_S2_IN:       .word   (1 << 27)
        BUTTON_S3_IN:       .word   (1 << 26)
        CHECK_BUTTON:       .word   0x0

        @ El led rojo está en GPIO44 y el verde en GPIO45
        LED_RED_MASK:       .word   (1 << (44-32))
        LED_GREEN_MASK:     .word   (1 << (45-32))
        LED_ON:             .word   0x0

        @ Retardo para el parpadeo
        DELAY:              .word   0x00080000

