/*****************************************************************************/
/*                                                                           */
/* Sistemas Empotrados                                                       */
/*                                                                           */
/* Linker script para la Redwire EconoTAG                                    */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************/
/* Punto de entrada                                                          */
/*****************************************************************************/

ENTRY(_start)

/*
 * Mapa de memoria de la placa
 */
MEMORY
{
        ram0 :  org = 0x00400000,       len = 0x00002000        /*  (8 KB)  */
        ram1 :  org = 0x00402000,       len = 0x00006000        /* (24 KB)  */
        ram2 :  org = 0x00408000,       len = 0x00008000        /* (32 KB)  */
        ram3 :  org = 0x00410000,       len = 0x00008000        /* (32 KB)  */
        gpio :  org = 0x80000000,       len = 0x00000400        /*  (1 KB)  */
}

SECTIONS
{
        /* Código del programa */
        .text : ALIGN(4)
        {
                *(.text);
        } > ram0

        /* Regiones de testeo de RAM */
        /*
        .ram1 :
        {
                s1 = .;     // Los espacios a cada lado del "=" son importantes
        } > ram1
        .ram2 :
        {
                s2 = .;     // Los espacios a cada lado del "=" son importantes
        } > ram2
        .ram3 :
        {
                s3 = .;     // Los espacios a cada lado del "=" son importantes
        } > ram3
        */

        .data : /*ALIGN(4)*/
        {
                *(.data);
                . = ALIGN(4);
        } > ram0

        .bss : /*ALIGN(4)*/
        {
                *(.bss);
                . = ALIGN(4);
        } > ram0

        .gpio : ALIGN(4)
        {
                /* Control de dirección */
                GPIO_PAD_DIR0 = . + 0x00;
                GPIO_PAD_DIR1 = . + 0x04;

                /* Datos */
                GPIO_DATA0 = . + 0x08;
                GPIO_DATA1 = . + 0x0c;

                /* Activación de bits */
                GPIO_DATA_SET0 = . + 0x48;
                GPIO_DATA_SET1 = . + 0x4c;

                /* Limpieza de bits */
                GPIO_DATA_RESET0 = . + 0x50;
                GPIO_DATA_RESET1 = . + 0x54;
        } > gpio
}
