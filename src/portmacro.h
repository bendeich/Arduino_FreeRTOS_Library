/*
 * FreeRTOS Kernel V10.3.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
*/

#ifndef PORTMACRO_H
#define PORTMACRO_H

#ifdef __cplusplus
extern "C" {
#endif

/*-----------------------------------------------------------
 * Port specific definitions.
 *
 * The settings in this file configure FreeRTOS correctly for the
 * given hardware and compiler.
 *
 * These settings should not be altered.
 *-----------------------------------------------------------
 */

/* Type definitions. */

typedef uint8_t                     StackType_t;
typedef int8_t                      BaseType_t;
typedef uint8_t                     UBaseType_t;

#if configUSE_16_BIT_TICKS == 1
    typedef uint16_t                TickType_t;
    #define portMAX_DELAY           ( TickType_t ) 0xffff
#else
    typedef uint32_t                TickType_t;
    #define portMAX_DELAY           ( TickType_t ) 0xffffffffUL
#endif
/*-----------------------------------------------------------*/

/* Critical section management. */

#define portENTER_CRITICAL()        __asm__ __volatile__ (                          \
                                        "in __tmp_reg__, __SREG__"        "\n\t"    \
                                        "cli"                             "\n\t"    \
                                        "push __tmp_reg__"                "\n\t"    \
                                        ::: "memory"                                \
                                        )


#define portEXIT_CRITICAL()         __asm__ __volatile__ (                          \
                                        "pop __tmp_reg__"                 "\n\t"    \
                                        "out __SREG__, __tmp_reg__"       "\n\t"    \
                                        ::: "memory"                                \
                                        )


#define portDISABLE_INTERRUPTS()    __asm__ __volatile__ ( "cli" ::: "memory")
#define portENABLE_INTERRUPTS()     __asm__ __volatile__ ( "sei" ::: "memory")
/*-----------------------------------------------------------*/

/* Architecture specifics. */

#define sleep_reset()                   do { _SLEEP_CONTROL_REG = 0; } while(0)     /* reset all sleep_mode() configurations. */

#define portSTACK_GROWTH            ( -1 )

/* Timing for the scheduler.
 */
#define portTICK_PERIOD_MS          ( (TickType_t) 10 )

#define portBYTE_ALIGNMENT          1
#define portNOP()                   __asm__ __volatile__ ( "nop" );
/*-----------------------------------------------------------*/

/* Kernel utilities. */
extern void vPortYield( void )      __attribute__ ( ( naked ) );
#define portYIELD()                 vPortYield()

extern void vPortYieldFromISR( void )   __attribute__ ( ( naked ) );
#define portYIELD_FROM_ISR()            vPortYieldFromISR()
/*-----------------------------------------------------------*/

#if defined(__AVR_3_BYTE_PC__)
/* Task function macros as described on the FreeRTOS.org WEB site. */

/* Add .lowtext tag from the avr linker script avr6.x for ATmega2560 and ATmega2561
 * to make sure functions are loaded in low memory.
 */
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters ) __attribute__ ((section (".lowtext")))
#else
#define portTASK_FUNCTION_PROTO( vFunction, pvParameters ) void vFunction( void *pvParameters )
#endif

#define portTASK_FUNCTION( vFunction, pvParameters ) void vFunction( void *pvParameters )

/* Tickless idle/low power functionality. */
#if( configUSE_TICKLESS_IDLE > 0 )
    #ifndef portSUPPRESS_TICKS_AND_SLEEP
        extern void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime);
        #define portSUPPRESS_TICKS_AND_SLEEP(xExpectedIdleTime) vPortSuppressTicksAndSleep(xExpectedIdleTime)
    #endif
#endif

#ifdef __cplusplus
}
#endif

#endif /* PORTMACRO_H */

