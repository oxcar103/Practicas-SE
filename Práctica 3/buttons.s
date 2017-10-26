@
@ Sistemas Empotrados
@

@
@ Variables globales
@
        .data

        @ La salida del botón S2 es GPIO23 y el del botón S3 es GPIO22
        BUTTON_S2_OUT:      .word   (1 << 23)
        BUTTON_S3_OUT:      .word   (1 << 22)

        @ La entrada del botón S2 es GPIO27 y el del botón S3 es GPIO26
        BUTTON_S2_IN:       .word   (1 << 27)
        BUTTON_S3_IN:       .word   (1 << 26)
        
        @ El led rojo está en GPIO44 y el verde en GPIO45
        LED_RED_MASK:       .word   (1 << (44-32))
        LED_GREEN_MASK:     .word   (1 << (45-32))

        @ Retardo para el parpadeo
        DELAY:              .word   0x00080000

        .bss
        @ Botón a comprobar
        CHECK_BUTTON:       .word   0x0
        @ Led a encender
        LED_ON:             .word   0x0

@
@ Punto de entrada
@

        .code 32
        .text
        .global _start
        .type   _start, %function

_start:
        bl      gpio_init

        @ Inicializamos la pila
        ldr      sp, =_stack_top

        @ Fijamos los valores iniciales de las variables que usaremos
        ldr     r4, =BUTTON_S3_IN
        ldr     r5, [r4]
        ldr     r4, =CHECK_BUTTON
        str     r5, [r4]
        
        ldr     r4, =LED_RED_MASK
        ldr     r5, [r4]
        ldr     r4, =LED_ON
        str     r5, [r4]

loop:
        @ Comprobamos botones pulsados
        bl      test_buttons

        @ Encendemos el led
        ldr     r4, =LED_ON
        ldr     r5, [r4]
        ldr     r4, =GPIO_DATA_SET1
        str     r5, [r4]

        @ Pausa corta
        ldr     r4, =DELAY
        ldr     r0, [r4]
        bl      pause

        @ Apagamos el led
        ldr     r4, =LED_ON
        ldr     r5, [r4]
        ldr     r4, =GPIO_DATA_RESET1
        str     r5, [r4]

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
        ldr     r2, =GPIO_DATA_SET0

        @ Establecemos a 1 la salida de los GPIO22 y GPIO23
        ldr     r3, =BUTTON_S2_OUT
        ldr     r0, [r3]
        ldr     r3, =BUTTON_S3_OUT
        ldr     r1, [r3]
        orr     r0, r0, r1
        str     r0, [r2]

        @ Configuramos GPIO44 y GPIO45 como salida
        ldr     r2, =GPIO_PAD_DIR1
        ldr     r3, =LED_RED_MASK
        ldr     r0, [r3]
        ldr     r3, =LED_GREEN_MASK
        ldr     r1, [r3]
        orr     r0, r0, r1
        str     r0, [r2]

        @ Retornamos a donde se invocó la función
        mov     pc, lr

@
@ Función para detectar si un botón se pulsa
@
        .type   test_buttons, %function
test_buttons:
        @ Comprobamos si el botón está pulsado
        ldr     r3, =GPIO_DATA0        
        ldr     r0, [r3]
        ldr     r3, =CHECK_BUTTON
        ldr     r1, [r3]
        tst     r0, r1

        @ Si lo está, cambiamos el led que debemos encender y el botón que debemos comprobar en otra función
        push    {lr}
        blne    pressed_button
        pop     {lr}

        @ Retornamos a donde se invocó la función
        mov     pc, lr

@
@ Función para cambiar CHECK_BUTTON y LED_ON dado que se ha pulsado el botón.
@
        .type   pressed_button, %function
pressed_button:
        @ Sumamos ambas máscaras
        ldr     r3, =BUTTON_S2_IN
        ldr     r0, [r3]
        ldr     r3, =BUTTON_S3_IN
        ldr     r1, [r3]
        orr     r0, r0, r1

        @ Permutamos el valor de CHECK_BUTTON
        ldr     r3, =CHECK_BUTTON
        ldr     r2, [r3]
        eor     r0, r0, r2
        str     r0, [r3]

        @ Sumamos ambas máscaras
        ldr     r3, =LED_RED_MASK
        ldr     r0, [r3]
        ldr     r3, =LED_GREEN_MASK
        ldr     r1, [r3]
        orr     r0, r0, r1

        @ Permutamos el valor de LED_ON
        ldr     r3, =LED_ON
        ldr     r2, [r3]
        eor     r0, r0, r2
        str     r0, [r3]

        @ Retornamos a donde se invocó la función
        mov     pc, lr

