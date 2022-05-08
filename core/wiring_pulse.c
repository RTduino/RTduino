/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-12     Meco Man     first version
 */

#include <rtdevice.h>
#include "wiring_private.h"

#define DBG_TAG    "Arduino"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

static struct rt_semaphore pulsein_sem;
static rt_bool_t pulsein_sem_init_flag = RT_FALSE;

struct
{
    rt_bool_t first_pulse_coming;
    rt_hwtimerval_t first_pulse_timestamp;
    rt_bool_t second_pulse_coming;
    rt_hwtimerval_t second_pulse_timestamp;
    rt_uint8_t state;
    rt_int32_t rt_isr_pin;
    rt_device_t hwdevice;
}pulse_record;


static void pulsein_pin_interrupt_cb(void *args)
{
    rt_hwtimerval_t timestamp;

    rt_device_read(pulse_record.hwdevice, 0, &timestamp, sizeof(timestamp));

    if(pulse_record.first_pulse_coming == RT_FALSE
        && pulse_record.second_pulse_coming == RT_FALSE
        && rt_pin_read(pulse_record.rt_isr_pin) == pulse_record.state)
    {
        pulse_record.first_pulse_timestamp.sec = timestamp.sec;
        pulse_record.first_pulse_timestamp.usec = timestamp.usec;
        pulse_record.first_pulse_coming = RT_TRUE;
    }
    else if(pulse_record.first_pulse_coming == RT_TRUE
        && pulse_record.second_pulse_coming == RT_FALSE
        && rt_pin_read(pulse_record.rt_isr_pin) != pulse_record.state)
    {
        pulse_record.second_pulse_timestamp.sec = timestamp.sec;
        pulse_record.second_pulse_timestamp.usec = timestamp.usec;
        pulse_record.second_pulse_coming = RT_TRUE;
        rt_sem_release(&pulsein_sem); /* have waitted the second pulse */
    }
    else
    {
        /* logic error, skip */
    }
}

/* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse.
 *
 * This function performs better with short pulses in noInterrupt() context
 */
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
    rt_int32_t rt_pin;
    rt_err_t rt_err;
    long delta;

    RT_ASSERT(state == HIGH || state == LOW);

    pulse_record.hwdevice = rt_device_find(RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME);
    if(pulse_record.hwdevice == RT_NULL)
    {
        LOG_E("Cannot find hardware timer!");
        return 0;
    }

    if(pulsein_sem_init_flag == RT_FALSE)
    {
        rt_sem_init(&pulsein_sem, "pulsein", 0, RT_IPC_FLAG_PRIO);
        pulsein_sem_init_flag = RT_TRUE;
    }

    rt_pin = pin_map_table[pin].rt_pin;

    /* clear parameters */
    pulsein_sem.value = 0; /* clear sem value */
    pulse_record.first_pulse_coming = RT_FALSE;
    pulse_record.first_pulse_timestamp.sec = 0;
    pulse_record.first_pulse_timestamp.usec = 0;
    pulse_record.second_pulse_coming = RT_FALSE;
    pulse_record.second_pulse_timestamp.sec = 0;
    pulse_record.second_pulse_timestamp.usec = 0;
    pulse_record.state = state;
    pulse_record.rt_isr_pin = rt_pin;

    rt_pin_attach_irq(rt_pin, PIN_IRQ_MODE_RISING_FALLING, pulsein_pin_interrupt_cb, RT_NULL);
    rt_pin_irq_enable(rt_pin, PIN_IRQ_ENABLE); /* enable interrupt */
    rt_err = rt_sem_take(&pulsein_sem, rt_tick_from_millisecond(timeout/1000));
    rt_pin_irq_enable(rt_pin, PIN_IRQ_DISABLE); /* disable interrupt */

    if(rt_err == RT_EOK)
    {
        delta = (pulse_record.second_pulse_timestamp.sec - pulse_record.first_pulse_timestamp.sec)*1000000;
        delta += (pulse_record.second_pulse_timestamp.usec - pulse_record.first_pulse_timestamp.usec);
    }
    else
    {
        delta = 0;
    }
    return delta;
}

/* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse.
 *
 * ATTENTION:
 * this function relies on micros() so cannot be used in noInterrupt() context
 */
unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout)
{
    LOG_E("pulseInLong() has not been implemented yet!");
    return 0;
}
