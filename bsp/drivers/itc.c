/*
 * Sistemas operativos empotrados
 * Driver para el controlador de interrupciones del MC1322x
 */

#include "system.h"

/*****************************************************************************/

/**
 * Acceso estructurado a los registros de control del ITC del MC1322x
 */
typedef struct{
    uint32_t INTCNTL;       // Interrupt Control Register
    uint32_t NIMASK;        // Normal Interrupt Mask Register
    uint32_t INTENNUM;      // Interrupt Enable Number Register
    uint32_t INTDISNUM;     // Interrupt Disable Number Register
    uint32_t INTENABLE;     // Interrupt Enable Register
    uint32_t INTTYPE;       // Interrupt Type Register
    uint32_t reserved[4];   // Reserved
    uint32_t NIVECTOR;      // Normal Interrupt Vector
    uint32_t FIVECTOR;      // Fast Interrupt Vector
    uint32_t INTSRC;        // Interrupt Source Register
    uint32_t INTFRC;        // Interrupt Force Register
    uint32_t NIPEND;        // Normal Interrupt Pending Register
    uint32_t FIPEND;        // Fast Interrupt Pending Register
} itc_regs_t;

static volatile itc_regs_t* const itc_regs = ITC_BASE;

/**
 * Tabla de manejadores de interrupción.
 */
static itc_handler_t itc_handlers[itc_src_max];


/**
 * Almacena el estado de INTENABLE cuando se deshabilitan las interrupciones
 * para poder restaurarlo cuando se habiliten de nuevo.
 */
static uint32_t v_intenable;

/*****************************************************************************/

/**
 * Inicializa el controlador de interrupciones.
 * Deshabilita los bits I y F de la CPU, inicializa la tabla de manejadores a NULL,
 * y habilita el arbitraje de interrupciones Normales y rápidas en el controlador
 * de interupciones.
 */
inline void itc_init ()
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 6 */
}

/*****************************************************************************/

/**
 * Deshabilita el envío de peticiones de interrupción a la CPU
 * Permite implementar regiones críticas en modo USER
 */
inline void itc_disable_ints (){
    v_intenable = itc_regs->INTENABLE;
    itc_regs->INTENABLE = 0;
}

/*****************************************************************************/

/**
 * Vuelve a habilitar el envío de peticiones de interrupción a la CPU
 * Permite implementar regiones críticas en modo USER
 */
inline void itc_restore_ints (){
    itc_regs->INTENABLE = v_intenable;
}

/*****************************************************************************/

/**
 * Asigna un manejador de interrupción
 * @param src		Identificador de la fuente
 * @param handler	Manejador
 */
inline void itc_set_handler (itc_src_t src, itc_handler_t handler)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 6 */
}

/*****************************************************************************/

/**
 * Asigna una prioridad (normal o fast) a una fuente de interrupción
 * @param src		Identificador de la fuente
 * @param priority	Tipo de prioridad
 */
inline void itc_set_priority (itc_src_t src, itc_priority_t priority)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 6 */
}

/*****************************************************************************/

/**
 * Habilita las interrupciones de una determinda fuente
 * @param src		Identificador de la fuente
 */
inline void itc_enable_interrupt (itc_src_t src)
{
    itc_regs->INTENNUM = src
}

/*****************************************************************************/

/**
 * Deshabilita las interrupciones de una determinda fuente
 * @param src		Identificador de la fuente
 */
inline void itc_disable_interrupt (itc_src_t src)
{
    itc_regs->INTDISNUM = src
}

/*****************************************************************************/

/**
 * Fuerza una interrupción con propósitos de depuración
 * @param src		Identificador de la fuente
 */
inline void itc_force_interrupt (itc_src_t src)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 6 */
}

/*****************************************************************************/

/**
 * Desfuerza una interrupción con propósitos de depuración
 * @param src		Identificador de la fuente
 */
inline void itc_unforce_interrupt (itc_src_t src)
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 6 */
}

/*****************************************************************************/

/**
 * Da servicio a la interrupción normal pendiente de más prioridad.
 * En el caso de usar un manejador de excepciones IRQ que permita interrupciones
 * anidadas, debe deshabilitar las IRQ de menor prioridad hasta que se haya
 * completado el servicio de la IRQ para evitar inversiones de prioridad
 */
void itc_service_normal_interrupt ()
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 6 */
}

/*****************************************************************************/

/**
 * Da servicio a la interrupción rápida pendiente de más prioridad
 */
void itc_service_fast_interrupt ()
{
	/* ESTA FUNCIÓN SE DEFINIRÁ EN LA PRÁCTICA 6 */
}

/*****************************************************************************/
