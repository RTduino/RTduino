/* Arduino ARM M0+ Bareboard scheduler
 *
 * Copyright 2017 The MathWorks, Inc.
 */

#include "Arduino.h"
#include "arduinoARM_M0plusScheduler.h"
#include "Reset.h"
/* Multi tasking related header file */

#include "arm_cortex_m_multitasking.h"

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
static void Scheduler_Isr(void)
{
    DISABLE_SCHEDULER_INT();


    /* DSB (Data Synchronization Barrier) operation will complete when all
     * explicit memory accesses before this instruction have completed.
     * No instructions after the DSB will be executed until the DSB
     * instruction has completed, that is, when all of the pending accesses
     * have completed. */
   // __DSB();

    /* ISB (Instruction Synchronization Barrier)flushes the pipeline in
     * the processor, so that all instructions following the ISB are
     * fetched from cache or memory, after the instruction has been
     * completed. It ensures that changes to the system, for example
     * the MPU, take immediate effect.*/
   // __ISB();

    #if (defined(MW_SCHEDULERCOUNTER) && (MW_SCHEDULERCOUNTER > 1))
      if ((++scheduler_counter) == MW_SCHEDULERCOUNTER) {
         scheduler_counter = 0;
    #endif

    #ifdef _RTT_OVERRUN_DIGITAL_PIN_
        if (IsrOverrun == 1)
        {
          /* Overrun detected */
          digitalWrite(_RTT_OVERRUN_DIGITAL_PIN_, HIGH);
        }
    #endif

    rt_OneStep();

      #if (defined(MW_SCHEDULERCOUNTER) && (MW_SCHEDULERCOUNTER > 1))
      }
    #endif

    ENABLE_SCHEDULER_INT();
}

#ifndef _MW_ARDUINO_LOOP_
#ifdef _ARDUINO_MKR1000_TIMER_5_
__attribute__ ((naked)) void TC5_Handler(void)
#elif defined(_ARDUINO_MKR1000_TIMER_4_)
__attribute__ ((naked)) void TC4_Handler(void)
#elif defined(_ARDUINO_MKR1000_TIMER_3_)
__attribute__ ((naked)) void TC3_Handler(void)
#endif
{
    /* Multi tasking, enable re-entrancy */

    /* Naked functions need the following prolog */
    ARM_CORTEX_M_PUSH_THREADS_PSR_AND_RET_ADDR();

    /* Needs to be called to reenable the Timer interrupt */
    #if defined(_ARDUINO_MKR1000_TIMER_5_)
       TC5->COUNT16.INTFLAG.bit.MC0 = 1;
    #elif defined(_ARDUINO_MKR1000_TIMER_4_)
       TC4->COUNT16.INTFLAG.bit.MC0 = 1;
    #elif defined(_ARDUINO_MKR1000_TIMER_3_)
       TC3->COUNT16.INTFLAG.bit.MC0 = 1;
    #endif

    /* Context switch to rt_OneStep() */
    arm_cortex_m_call_thread_with_context_switch(Scheduler_Isr);
}

#endif

extern "C" void configureArduinoARM_M0plusTimer(void)
{
    #if defined(_ARDUINO_MKR1000_TIMER_5_)
      Setup_SchedulerTimer(TC_FOR_TIMER5, ID_TC_FOR_TIMER5, IRQn_FOR_TIMER5, MW_CLK_DIV, MW_TIMERCOUNT);
    #elif defined(_ARDUINO_MKR1000_TIMER_4_)
      Setup_SchedulerTimer(TC_FOR_TIMER4, ID_TC_FOR_TIMER4, IRQn_FOR_TIMER4, MW_CLK_DIV, MW_TIMERCOUNT);
    #elif defined(_ARDUINO_MKR1000_TIMER_3_)
      Setup_SchedulerTimer(TC_FOR_TIMER3, ID_TC_FOR_TIMER3, IRQn_FOR_TIMER3, MW_CLK_DIV, MW_TIMERCOUNT);
    #endif
}

void Setup_SchedulerTimer(Tc *tc, uint8_t timerId, IRQn_Type irqn, uint16_t prescaler, uint16_t tickCounter)
{
#ifndef _MW_ARDUINO_LOOP_
    /* Configure interrupt request*/
    NVIC_DisableIRQ(irqn);
    NVIC_ClearPendingIRQ(irqn);
    NVIC_SetPriority(irqn,(1<<__NVIC_PRIO_BITS) - 1);

    /*Enable GCLK for Timer TCx with source GCLK0 48MHz */
    GCLK->CLKCTRL.reg = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID(timerId)) ;

  /* Wait for sync */
  while (GCLK->STATUS.bit.SYNCBUSY);

  /* TCx reset */
    tc->COUNT16.CTRLA.reg = TC_CTRLA_SWRST;
    while (tc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY);
    while (tc->COUNT16.CTRLA.bit.SWRST);

  /* Set TCx Mode to 16 bits, match frequency, prescaler */
   tc->COUNT16.CTRLA.reg |= TC_CTRLA_MODE_COUNT16 | TC_CTRLA_WAVEGEN_MFRQ | prescaler ;
   /* Wait for sync */
    tc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;

    /* Load count */
    tc->COUNT16.CC[0].reg = (uint16_t)tickCounter;

    /* Wait for sync */
    tc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;

    /* Set TCx interrupt request high */
    tc->COUNT16.INTENSET.bit.MC0 = 1;

    /* Wait for sync */
    tc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;

   /*TC start counter: Enable TC*/
    tc->COUNT16.CTRLA.reg |= TC_CTRLA_ENABLE;

    /* Wait for sync */
    tc->COUNT16.STATUS.reg & TC_STATUS_SYNCBUSY;

    /*Enable interrupt request */
    NVIC_EnableIRQ(irqn);
#endif
}

void disable_rt_OneStep(void)
{
#ifndef _MW_ARDUINO_LOOP_
    // Disable the overflow interrupt for XCP External Mode
    #if defined(_ARDUINO_MKR1000_TIMER_5_)
        NVIC_DisableIRQ(IRQn_FOR_TIMER5);
    #elif defined(_ARDUINO_MKR1000_TIMER_4_)
        NVIC_DisableIRQ(IRQn_FOR_TIMER4);
    #elif defined(_ARDUINO_MKR1000_TIMER_3_)
        NVIC_DisableIRQ(IRQn_FOR_TIMER3);
    #endif
#endif
}

boolean MW_Arduino_Loop(void)
{
    boolean ret = 0;

#ifdef _MW_ARDUINO_LOOP_
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
    }
#endif

    ret = 1;
    return ret;
}

