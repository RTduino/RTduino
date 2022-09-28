/* Arduino ARM Bareboard scheduler
 *
 * Copyright 2013 The MathWorks, Inc.
 */

#ifndef _ARDUINOARMSCHEDULER_H_
#define _ARDUINOARMSCHEDULER_H_

#include "Arduino.h"

#ifdef _MSC_VER
#define __INLINE inline
#define __STATIC_INLINE static inline
#define __DSB()
#endif

#ifndef __STATIC_INLINE
#define __STATIC_INLINE static __INLINE
#endif

#if (defined(MW_TIMERID) && (MW_TIMERID == 7))
      #define _ARDUINO_DUE_TIMER_6_
#elif (defined(MW_TIMERID) && (MW_TIMERID == 8))
      #define _ARDUINO_DUE_TIMER_7_
#elif (defined(MW_TIMERID) && (MW_TIMERID == 9))
      #define _ARDUINO_DUE_TIMER_8_
#endif

// For SAM3X:
#if defined(_ARDUINO_DUE_TIMER_6_)
    #define TC_FOR_TIMER6       TC2
    #define CHANNEL_FOR_TIMER6  0
    #define ID_TC_FOR_TIMER6    ID_TC6
    #define IRQn_FOR_TIMER6     TC6_IRQn
    #define HANDLER_FOR_TIMER6  TC6_Handler
#endif

#if defined(_ARDUINO_DUE_TIMER_7_)
    #define TC_FOR_TIMER7       TC2
    #define CHANNEL_FOR_TIMER7  1
    #define ID_TC_FOR_TIMER7    ID_TC7
    #define IRQn_FOR_TIMER7     TC7_IRQn
    #define HANDLER_FOR_TIMER7  TC7_Handler
#endif

#if defined(_ARDUINO_DUE_TIMER_8_)
    #define TC_FOR_TIMER8       TC2
    #define CHANNEL_FOR_TIMER8  2
    #define ID_TC_FOR_TIMER8    ID_TC8
    #define IRQn_FOR_TIMER8     TC8_IRQn
    #define HANDLER_FOR_TIMER8  TC8_Handler
#endif

 #define DISABLE_SCHEDULER_INT()    (noInterrupts())
 #define ENABLE_SCHEDULER_INT()     (interrupts())


#ifdef __cplusplus
extern "C" {
#endif

void configureArduinoARMTimer(void);
void Setup_SchedulerTimer(Tc *tc, uint32_t channel, uint32_t id, IRQn_Type irqn, uint8_t clockid, uint32_t tickcounter);
boolean MW_Arduino_Loop(void);
void rt_OneStep(void);
void disable_rt_OneStep(void);

#ifdef __cplusplus
}
#endif

#endif // _ARDUINOARMSCHEDULER_H_
