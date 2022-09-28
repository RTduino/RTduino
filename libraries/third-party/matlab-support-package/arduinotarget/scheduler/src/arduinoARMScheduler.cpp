/* Arduino ARM M3 Bareboard scheduler
 *
 * Copyright 2013-2019 The MathWorks, Inc.
 */

#include "Arduino.h"
#include "arduinoARMScheduler.h"
#include "Reset.h"
/* Multi tasking related header file */
#include "arm_cortex_m_multitasking.h"

extern volatile int IsrOverrun;
unsigned long oldtime = 0L;
unsigned long actualtime;

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
    __DSB();

    /* ISB (Instruction Synchronization Barrier)flushes the pipeline in
     * the processor, so that all instructions following the ISB are
     * fetched from cache or memory, after the instruction has been
     * completed. It ensures that changes to the system, for example
     * the MPU, take immediate effect.*/
    __ISB();

    #ifdef _RTT_OVERRUN_DIGITAL_PIN_
        if (IsrOverrun == 1)
        {
          // Overrun detected
          digitalWrite(_RTT_OVERRUN_DIGITAL_PIN_, HIGH);
        }
    #endif

    rt_OneStep();

    ENABLE_SCHEDULER_INT();
}

#ifndef _MW_ARDUINO_LOOP_
#if defined(_ARDUINO_DUE_TIMER_6_)
MW_RENTRANT_ISR_FUNCTION_BEGIN(TC6_Handler, IRQn_FOR_TIMER6)
#elif defined(_ARDUINO_DUE_TIMER_7_)
MW_RENTRANT_ISR_FUNCTION_BEGIN(TC7_Handler, IRQn_FOR_TIMER7)
#elif defined(_ARDUINO_DUE_TIMER_8_)
MW_RENTRANT_ISR_FUNCTION_BEGIN(TC8_Handler, IRQn_FOR_TIMER8)
#endif
    /* Needs to be called to reenable the Timer interrupt */
    #if defined(_ARDUINO_DUE_TIMER_6_)
    TC_GetStatus(TC2, 0);
    #elif defined(_ARDUINO_DUE_TIMER_7_)
    TC_GetStatus(TC2, 1);
    #elif defined(_ARDUINO_DUE_TIMER_8_)
    TC_GetStatus(TC2, 2);
    #endif
    MW_CALL_ISR_FUNCTION(Scheduler_Isr);
MW_RENTRANT_ISR_FUNCTION_END()
#endif

void configureArduinoARMTimer(void)
{
    #if defined(_ARDUINO_DUE_TIMER_6_)
      Setup_SchedulerTimer(TC_FOR_TIMER6, CHANNEL_FOR_TIMER6, ID_TC_FOR_TIMER6, IRQn_FOR_TIMER6, MW_SAM_CLOCKID, MW_TIMERCOUNT);
    #elif defined(_ARDUINO_DUE_TIMER_7_)
      Setup_SchedulerTimer(TC_FOR_TIMER7, CHANNEL_FOR_TIMER7, ID_TC_FOR_TIMER7, IRQn_FOR_TIMER7, MW_SAM_CLOCKID, MW_TIMERCOUNT);
    #elif defined(_ARDUINO_DUE_TIMER_8_)
      Setup_SchedulerTimer(TC_FOR_TIMER8, CHANNEL_FOR_TIMER8, ID_TC_FOR_TIMER8, IRQn_FOR_TIMER8, MW_SAM_CLOCKID, MW_TIMERCOUNT);
    #endif
}

void Setup_SchedulerTimer(Tc *tc, uint32_t channel, uint32_t id, IRQn_Type irqn, uint8_t clockid, uint32_t tickcounter)
{
#ifndef _MW_ARDUINO_LOOP_
    pmc_set_writeprotect(false);
    pmc_enable_periph_clk(id);

    TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | clockid);

    TC_SetRC(tc, channel, tickcounter);

    TC_Start(tc, channel);

    tc->TC_CHANNEL[channel].TC_IER=TC_IER_CPCS;
    tc->TC_CHANNEL[channel].TC_IDR=~TC_IER_CPCS;

    NVIC_SetPriority(irqn, (1<<__NVIC_PRIO_BITS) - 1);
    NVIC_EnableIRQ(irqn);
#endif
}

void disable_rt_OneStep(void)
{
#ifndef _MW_ARDUINO_LOOP_
  // Disable the overflow interrupt for XCP External Mode
    #if defined(_ARDUINO_DUE_TIMER_6_)
        NVIC_DisableIRQ(IRQn_FOR_TIMER6);
    #elif defined(_ARDUINO_DUE_TIMER_7_)
        NVIC_DisableIRQ(IRQn_FOR_TIMER7);
    #elif defined(_ARDUINO_DUE_TIMER_8_)
        NVIC_DisableIRQ(IRQn_FOR_TIMER8);
    #endif
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

