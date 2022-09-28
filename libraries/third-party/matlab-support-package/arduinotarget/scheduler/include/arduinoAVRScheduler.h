/* Arduino Bareboard scheduler
 *
 * Copyright 2013-2015 The MathWorks, Inc.
 */

#ifndef _ARDUINOSCHEDULER_H_
#define _ARDUINOSCHEDULER_H_

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

void configureArduinoAVRTimer(void);
void disable_rt_OneStep(void);
boolean MW_Arduino_Loop(void);
void rt_OneStep(void);

#ifdef __cplusplus
}
#endif

#define INIT_TIMER_VAL  MW_TIMERCOUNT

#if (defined(MW_TIMERID) && (MW_TIMERID == 1))
    #if (defined(MW_PRESCALAR))
        #if (MW_PRESCALAR==1024)
             #define SETUP_PRESCALER   TCCR1B |= ((1<<CS12) | (1<<CS10)); \
                                       TCCR1B &= ~((1<<CS11))
        #elif (MW_PRESCALAR==256)
             #define SETUP_PRESCALER   TCCR1B |= ((1<<CS12)); \
                                       TCCR1B &= ~((1<<CS11) | (1<<CS10))
        #elif (MW_PRESCALAR==64)
             #define SETUP_PRESCALER   TCCR1B |= ((1<<CS11) | (1<<CS10)); \
                                       TCCR1B &= ~((1<<CS12))
        #elif (MW_PRESCALAR==8)
             #define SETUP_PRESCALER   TCCR1B |= ((1<<CS11)); \
                                       TCCR1B &= ~((1<<CS12) | (1<<CS10))
        #elif (MW_PRESCALAR==1)
             #define SETUP_PRESCALER   TCCR1B |= ((1<<CS10)); \
                                       TCCR1B &= ~((1<<CS12) | (1<<CS11))
        #endif
    #endif

      #define INTERRUPT_VECTOR    TIMER1_OVF_vect
      #define DISABLE_SCHEDULER_INT()       TIMSK1 &= ~(1<<TOIE1)
      #define ENABLE_SCHEDULER_INT()        TIMSK1 |= (1<<TOIE1)
      #define TIMER_NORMAL_MODE   TCCR1A &= ~((1<<WGM11) | (1<<WGM10)); \
                                  TCCR1B &= ~((1<<WGM13) | (1<<WGM12))
      #define RESET_TIMER         TCNT1 = INIT_TIMER_VAL

#elif (defined(MW_TIMERID) && (MW_TIMERID == 2))
    #if (defined(MW_PRESCALAR))
        #if (MW_PRESCALAR==1024)
              #define SETUP_PRESCALER   TCCR2B |= ((1<<CS22) | (1<<CS21) | (1<<CS20))

        #elif (MW_PRESCALAR==256)
              #define SETUP_PRESCALER   TCCR2B |= ((1<<CS22) | (1<<CS21)); \
                                        TCCR2B &= ~(1<<CS20)
        #elif (MW_PRESCALAR==128)
              #define SETUP_PRESCALER   TCCR2B |= ((1<<CS22) | (1<<CS20)); \
                                        TCCR2B &= ~((1<<CS21))
        #elif (MW_PRESCALAR==64)
              #define SETUP_PRESCALER   TCCR2B |= ((1<<CS22)); \
                                        TCCR2B &= ~((1<<CS21) | (1<<CS20))
        #elif (MW_PRESCALAR==32)
              #define SETUP_PRESCALER   TCCR2B |= ((1<<CS21) | (1<<CS20)); \
                                        TCCR2B &= ~((1<<CS22))
        #elif (MW_PRESCALAR==8)
              #define SETUP_PRESCALER   TCCR2B |= ((1<<CS21)); \
                                        TCCR2B &= ~((1<<CS22) | (1<<CS20))
        #elif (MW_PRESCALAR==1)
              #define SETUP_PRESCALER   TCCR2B |= ((1<<CS20)); \
                                        TCCR2B &= ~((1<<CS22) | (1<<CS21))
        #endif
    #endif

      #define INTERRUPT_VECTOR    TIMER2_OVF_vect
      #define DISABLE_SCHEDULER_INT()       TIMSK2 &= ~(1<<TOIE2)
      #define ENABLE_SCHEDULER_INT()        TIMSK2 |= (1<<TOIE2)
      #define TIMER_NORMAL_MODE   TCCR2A &= ~((1<<WGM21) | (1<<WGM20)); \
                                  TCCR2B &= ~(1<<WGM22)
      #define RESET_TIMER         TCNT2 = INIT_TIMER_VAL

#elif (defined(MW_TIMERID) && (MW_TIMERID == 3))
    #if (defined(MW_PRESCALAR))
        #if (MW_PRESCALAR==1024)
              #define SETUP_PRESCALER   TCCR3B |= ((1<<CS32) | (1<<CS30)); \
                                        TCCR3B &= ~((1<<CS31))
        #elif (MW_PRESCALAR==256)
              #define SETUP_PRESCALER   TCCR3B |= ((1<<CS32)); \
                                        TCCR3B &= ~((1<<CS31) | (1<<CS30))
        #elif (MW_PRESCALAR==64)
              #define SETUP_PRESCALER   TCCR3B |= ((1<<CS31) | (1<<CS30)); \
                                        TCCR3B &= ~((1<<CS32))
        #elif (MW_PRESCALAR==8)
              #define SETUP_PRESCALER   TCCR3B |= ((1<<CS31)); \
                                        TCCR3B &= ~((1<<CS32) | (1<<CS30))
        #elif (MW_PRESCALAR==1)
              #define SETUP_PRESCALER   TCCR3B |= ((1<<CS30)); \
                                        TCCR3B &= ~((1<<CS32) | (1<<CS31))
        #endif
    #endif

      #define INTERRUPT_VECTOR    TIMER3_OVF_vect
      #define DISABLE_SCHEDULER_INT()       TIMSK3 &= ~(1<<TOIE3)
      #define ENABLE_SCHEDULER_INT()        TIMSK3 |= (1<<TOIE3)
      #define TIMER_NORMAL_MODE   TCCR3A &= ~((1<<WGM31) | (1<<WGM30)); \
                                  TCCR3B &= ~((1<<WGM33) | (1<<WGM32))
      #define RESET_TIMER         TCNT3 = INIT_TIMER_VAL

#elif (defined(MW_TIMERID) && (MW_TIMERID == 4))
    #if (defined(MW_PRESCALAR))
        #if (MW_PRESCALAR==1024)
              #define SETUP_PRESCALER   TCCR4B |= ((1<<CS42) | (1<<CS40)); \
                                        TCCR4B &= ~((1<<CS41))
        #elif (MW_PRESCALAR==256)
              #define SETUP_PRESCALER   TCCR4B |= ((1<<CS42)); \
                                        TCCR4B &= ~((1<<CS41) | (1<<CS40))
        #elif (MW_PRESCALAR==64)
              #define SETUP_PRESCALER   TCCR4B |= ((1<<CS41) | (1<<CS40)); \
                                        TCCR4B &= ~((1<<CS42))
        #elif (MW_PRESCALAR==8)
              #define SETUP_PRESCALER   TCCR4B |= ((1<<CS41)); \
                                        TCCR4B &= ~((1<<CS42) | (1<<CS40))
        #elif (MW_PRESCALAR==1)
              #define SETUP_PRESCALER   TCCR4B |= ((1<<CS40)); \
                                        TCCR4B &= ~((1<<CS42) | (1<<CS41))
        #endif
    #endif

      #define INTERRUPT_VECTOR    TIMER4_OVF_vect
      #define DISABLE_SCHEDULER_INT()       TIMSK4 &= ~(1<<TOIE4)
      #define ENABLE_SCHEDULER_INT()        TIMSK4 |= (1<<TOIE4)
      #define TIMER_NORMAL_MODE   TCCR4A &= ~((1<<WGM41) | (1<<WGM40)); \
                                  TCCR4B &= ~((1<<WGM43) | (1<<WGM42))
      #define RESET_TIMER         TCNT4 = INIT_TIMER_VAL

#elif (defined(MW_TIMERID) && (MW_TIMERID == 5))
    #if (defined(MW_PRESCALAR))
        #if (MW_PRESCALAR==1024)
              #define SETUP_PRESCALER   TCCR5B |= ((1<<CS52) | (1<<CS50)); \
                                        TCCR5B &= ~((1<<CS51))
        #elif (MW_PRESCALAR==256)
              #define SETUP_PRESCALER   TCCR5B |= ((1<<CS52)); \
                                        TCCR5B &= ~((1<<CS51) | (1<<CS50))
        #elif (MW_PRESCALAR==64)
              #define SETUP_PRESCALER   TCCR5B |= ((1<<CS51) | (1<<CS50)); \
                                        TCCR5B &= ~((1<<CS52))
        #elif (MW_PRESCALAR==8)
              #define SETUP_PRESCALER   TCCR5B |= ((1<<CS51)); \
                                        TCCR5B &= ~((1<<CS52) | (1<<CS50))
        #elif (MW_PRESCALAR==1)
              #define SETUP_PRESCALER   TCCR5B |= ((1<<CS50)); \
                                        TCCR5B &= ~((1<<CS52) | (1<<CS51))
        #endif
    #endif

      #define INTERRUPT_VECTOR    TIMER5_OVF_vect
      #define DISABLE_SCHEDULER_INT()       TIMSK5 &= ~(1<<TOIE5)
      #define ENABLE_SCHEDULER_INT()        TIMSK5 |= (1<<TOIE5)
      #define TIMER_NORMAL_MODE   TCCR5A &= ~((1<<WGM51) | (1<<WGM50)); \
                                  TCCR5B &= ~((1<<WGM53) | (1<<WGM52))
      #define RESET_TIMER         TCNT5 = INIT_TIMER_VAL

#elif (defined(MW_TIMERID) && (MW_TIMERID == 6))
    #if (defined(MW_PRESCALAR))
        #if (MW_PRESCALAR==16384)
          #define SETUP_PRESCALER   TCCR4B = (1<<CS43) |(1<<CS42) | (1<<CS41) | (1<<CS40)
        #elif (MW_PRESCALAR==8192)
          #define SETUP_PRESCALER   TCCR4B = (1<<CS43) |(1<<CS42) | (1<<CS41) | (0<<CS40)
        #elif (MW_PRESCALAR==4096)
          #define SETUP_PRESCALER   TCCR4B = (1<<CS43) |(1<<CS42) | (0<<CS41) | (1<<CS40)
        #elif (MW_PRESCALAR==2048)
          #define SETUP_PRESCALER   TCCR4B = (1<<CS43) |(1<<CS42) | (0<<CS41) | (0<<CS40)
        #elif (MW_PRESCALAR==1024)
          #define SETUP_PRESCALER   TCCR4B = (1<<CS43) |(0<<CS42) | (1<<CS41) | (1<<CS40)
        #elif (MW_PRESCALAR==512)
          #define SETUP_PRESCALER   TCCR4B = (1<<CS43) |(0<<CS42) | (1<<CS41) | (0<<CS40)
        #elif (MW_PRESCALAR==256)
          #define SETUP_PRESCALER   TCCR4B = (1<<CS43) |(0<<CS42) | (0<<CS41) | (1<<CS40)
        #elif (MW_PRESCALAR==128)
          #define SETUP_PRESCALER   TCCR4B = (1<<CS43) |(0<<CS42) | (0<<CS41) | (0<<CS40)
        #elif (MW_PRESCALAR==64)
          #define SETUP_PRESCALER   TCCR4B = (0<<CS43) |(1<<CS42) | (1<<CS41) | (1<<CS40)
        #elif (MW_PRESCALAR==32)
          #define SETUP_PRESCALER   TCCR4B = (0<<CS43) |(1<<CS42) | (1<<CS41) | (0<<CS40)
        #elif (MW_PRESCALAR==16)
          #define SETUP_PRESCALER   TCCR4B = (0<<CS43) |(1<<CS42) | (0<<CS41) | (1<<CS40)
        #elif (MW_PRESCALAR==8)
          #define SETUP_PRESCALER   TCCR4B = (0<<CS43) |(1<<CS42) | (0<<CS41) | (0<<CS40)
        #elif (MW_PRESCALAR==4)
          #define SETUP_PRESCALER   TCCR4B = (0<<CS43) |(0<<CS42) | (1<<CS41) | (1<<CS40)
        #elif (MW_PRESCALAR==2)
          #define SETUP_PRESCALER   TCCR4B = (0<<CS43) |(0<<CS42) | (1<<CS41) | (0<<CS40)
        #elif (MW_PRESCALAR==1)
          #define SETUP_PRESCALER   TCCR4B = (0<<CS43) |(0<<CS42) | (0<<CS41) | (1<<CS40)
        #endif
    #endif

      #define INTERRUPT_VECTOR    TIMER4_OVF_vect
      #define DISABLE_SCHEDULER_INT()       TIMSK4 &= ~(1<<TOIE4)
      #define ENABLE_SCHEDULER_INT()        TIMSK4 |= (1<<TOIE4)
      #define TIMER_NORMAL_MODE   TCCR4A = TCCR4C = TCCR4D = TCCR4E = 0
      #define RESET_TIMER         TCNT4 = 0
      #define LOAD_COMPARE_VALUE  TC4H = ((INIT_TIMER_VAL >> 8) & 0xFF); \
                                  OCR4C = (INIT_TIMER_VAL & 0xFF)

#endif

#endif // _ARDUINOSCHEDULER_H_
