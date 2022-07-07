/* Arduino AVR Bareboard scheduler
 *
 * Copyright 2013-2015 The MathWorks, Inc.
 */

#include <Arduino.h>
#include "../include/RTduinoScheduler.h"

extern volatile int IsrOverrun;
unsigned long oldtime = 0L;
unsigned long actualtime;


#if (defined(MW_SCHEDULERCOUNTER) &&  (MW_SCHEDULERCOUNTER > 1))
   #if (MW_SCHEDULERCOUNTER >= 65536)
      volatile uint32_t scheduler_counter = 0;
    #elif (MW_SCHEDULERCOUNTER >= 256)
      volatile uint16_t scheduler_counter = 0;
    #else
      volatile uint8_t scheduler_counter = 0;
    #endif
#endif

/*
 * Call the rt_Onestep function when the timer interrupt hits
 *
 */
#ifndef _MW_ARDUINO_LOOP_
ISR(INTERRUPT_VECTOR)
{
    RESET_TIMER;

    #if (defined(MW_SCHEDULERCOUNTER) && (MW_SCHEDULERCOUNTER > 1))
      if ((++scheduler_counter) == MW_SCHEDULERCOUNTER) {
         scheduler_counter = 0;
    #endif

    #ifdef _RTT_OVERRUN_DIGITAL_PIN_
        if (IsrOverrun == 1)
        {
          // Overrun detected
          digitalWrite(_RTT_OVERRUN_DIGITAL_PIN_, HIGH);
        }
    #endif
      rt_OneStep();

    #if (defined(MW_SCHEDULERCOUNTER) && (MW_SCHEDULERCOUNTER > 1))
      }
    #endif
}
#endif

void configureArduinoAVRTimer(void)
{
#ifndef _MW_ARDUINO_LOOP_
  // Sets up the timer overflow interrupt.
  RESET_TIMER;

  // Initially disable the overflow interrupt (before configuration).
  DISABLE_SCHEDULER_INT();

  // Set the timer to normal mode.
  TIMER_NORMAL_MODE;

  // Set the prescaler.
  SETUP_PRESCALER;

  // Enable the overflow interrupt.
  ENABLE_SCHEDULER_INT();
#endif
}

void disable_rt_OneStep(void)
{
#ifndef _MW_ARDUINO_LOOP_
  // Disable the overflow interrupt for XCP External Mode
  DISABLE_SCHEDULER_INT();
#endif
}

boolean MW_Arduino_Loop(void)
{
    boolean ret = 0;

#ifdef _MW_ARDUINO_LOOP_
    unsigned long overruntime;
#if (defined(MW_ARDUINO_MICROS))
    actualtime = micros();
#else
    actualtime = millis();
#endif

    if ((unsigned long) (actualtime - oldtime) >= MW_ARDUINO_STEP_SIZE)
    {
        oldtime = actualtime;

#ifdef _RTT_OVERRUN_DIGITAL_PIN_
        if (IsrOverrun == 1)
        {
            // Overrun detected
            digitalWrite(_RTT_OVERRUN_DIGITAL_PIN_, HIGH);
        }
#endif
        rt_OneStep();
            #if (defined(_MW_ARDUINO_LOOP_))
                #if (defined(MW_ARDUINO_MICROS))
                        overruntime = micros();
                #else
                        overruntime = millis();
                #endif
                if ((unsigned long) (overruntime - oldtime) >= MW_ARDUINO_STEP_SIZE)
                  IsrOverrun = 1;
                else
                  IsrOverrun = 0;
            #endif
}
#endif

    ret = 1;
    return ret;
}

