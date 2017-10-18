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
        ram1 :  org = 0x00402000,       len = 0x00006000        /* (24 KB) */
        ram2 :  org = 0x00408000,       len = 0x00008000        /* (32 KB)  */
        ram3 :  org = 0x00410000,       len = 0x00008000        /* (32 KB)  */
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
}
