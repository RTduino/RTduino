/* Arduino ARM M0+ Bareboard scheduler
 *
 * Copyright 2017 The MathWorks, Inc.
 */

#ifndef _ARDUINOARMM0PLUSSCHEDULER_H_
#define _ARDUINOARMM0PLUSSCHEDULER_H_

#include "Arduino.h"

#ifdef _MSC_VER
#define __INLINE inline
#define __STATIC_INLINE static inline
#define __DSB()
#endif

#ifndef __STATIC_INLINE
#define __STATIC_INLINE static __INLINE
#endif

#ifdef MW_PRESCALAR
    #if (MW_PRESCALAR == 1)
        #define MW_CLK_DIV TC_CTRLA_PRESCALER_DIV1
    #elif (MW_PRESCALAR == 2)
        #define MW_CLK_DIV TC_CTRLA_PRESCALER_DIV2
    #elif (MW_PRESCALAR == 4)
        #define MW_CLK_DIV TC_CTRLA_PRESCALER_DIV4
    #elif (MW_PRESCALAR == 8)
        #define MW_CLK_DIV TC_CTRLA_PRESCALER_DIV8
    #elif (MW_PRESCALAR == 16)
        #define MW_CLK_DIV TC_CTRLA_PRESCALER_DIV16
    #elif (MW_PRESCALAR == 64)
        #define MW_CLK_DIV TC_CTRLA_PRESCALER_DIV64
    #elif (MW_PRESCALAR == 256)
        #define MW_CLK_DIV TC_CTRLA_PRESCALER_DIV256
    #elif (MW_PRESCALAR == 1024)
        #define MW_CLK_DIV TC_CTRLA_PRESCALER_DIV1024
    #endif
#endif


#if (defined(MW_TIMERID) && (MW_TIMERID == 9))
      #define _ARDUINO_MKR1000_TIMER_5_
#elif (defined(MW_TIMERID) && (MW_TIMERID == 8))
       #define _ARDUINO_MKR1000_TIMER_4_
#elif (defined(MW_TIMERID) && (MW_TIMERID == 7))
       #define _ARDUINO_MKR1000_TIMER_3_
#endif

// For SAMD21G:
#if defined(_ARDUINO_MKR1000_TIMER_5_)
    #define TC_FOR_TIMER5       TC5
    #define ID_TC_FOR_TIMER5    GCM_TC4_TC5
    #define IRQn_FOR_TIMER5     TC5_IRQn
    #define HANDLER_FOR_TIMER5  TC5_Handler
#endif

#if defined(_ARDUINO_MKR1000_TIMER_4_)
    #define TC_FOR_TIMER4       TC4
    #define ID_TC_FOR_TIMER4    GCM_TC4_TC5
    #define IRQn_FOR_TIMER4     TC4_IRQn
    #define HANDLER_FOR_TIMER4  TC4_Handler
#endif

#if defined(_ARDUINO_MKR1000_TIMER_3_)
    #define TC_FOR_TIMER3       TC3
    #define ID_TC_FOR_TIMER3    GCM_TCC2_TC3
    #define IRQn_FOR_TIMER3     TC3_IRQn
    #define HANDLER_FOR_TIMER3  TC3_Handler
#endif

 #define DISABLE_SCHEDULER_INT()    (noInterrupts())
 #define ENABLE_SCHEDULER_INT()     (interrupts())


#ifdef __cplusplus
extern "C" {
#endif

void configureArduinoARM_M0plusTimer(void);
void Setup_SchedulerTimer(Tc *tc, uint8_t timerId, IRQn_Type irqn, uint16_t prescaler, uint16_t tickCounter);
boolean MW_Arduino_Loop(void);
void rt_OneStep(void);
void disable_rt_OneStep(void);

#ifdef __cplusplus
}
#endif

#endif // _ARDUINOARMM0PLUSSCHEDULER_H_
