/*
 * Copyright (c) 2021-2024, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * https://github.com/RTduino/RTduino
 * https://gitee.com/rtduino/RTduino
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-12     Meco Man     first version
 */

#include <rtdevice.h>
#include "wiring_private.h"

#define DBG_TAG    "RTduino.pulse"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

static struct rt_semaphore pulsein_sem;
static rt_bool_t pulsein_sem_init_flag = RT_FALSE;
static struct
{
    rt_bool_t first_pulse_coming;
    unsigned long first_pulse_timestamp_us;
    rt_bool_t second_pulse_coming;
    unsigned long second_pulse_timestamp_us;
    rt_uint8_t state;
    rt_int32_t rt_isr_pin;
} pulse_record;

static void pulsein_pin_interrupt_cb(void *args)
{
    RT_UNUSED(args);

    if (pulse_record.first_pulse_coming == RT_FALSE
        && pulse_record.second_pulse_coming == RT_FALSE
        && rt_pin_read(pulse_record.rt_isr_pin) == pulse_record.state)
    {
        pulse_record.first_pulse_timestamp_us = micros();
        pulse_record.first_pulse_coming = RT_TRUE;
    }
    else if (pulse_record.first_pulse_coming == RT_TRUE
        && pulse_record.second_pulse_coming == RT_FALSE
        && rt_pin_read(pulse_record.rt_isr_pin) != pulse_record.state)
    {
        pulse_record.second_pulse_timestamp_us = micros();
        pulse_record.second_pulse_coming = RT_TRUE;
        rt_sem_release(&pulsein_sem); /* has waited the second pulse */
    }
    else
    {
        /* logic illegal, skip */
        LOG_D("pulsein_pin_interrupt logic illegal. Skip");
    }
}

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
    rt_base_t level;
    unsigned long delta;
    rt_int32_t rt_pin;
    rt_err_t rt_err;

    RT_ASSERT(state == HIGH || state == LOW);

    RTDUINO_CHECK_PIN_LIMIT_RETURN(pin, 0); /* with return value */

    if (pulsein_sem_init_flag == RT_FALSE)
    {
        rt_sem_init(&pulsein_sem, "pulsein", 0, RT_IPC_FLAG_PRIO);
        pulsein_sem_init_flag = RT_TRUE;
    }

    rt_pin = pin_map_table[pin].rt_pin;

    /* clear parameters */
    level = rt_hw_interrupt_disable();
    pulsein_sem.value = 0; /* clear sem value */
    pulse_record.first_pulse_coming = RT_FALSE;
    pulse_record.first_pulse_timestamp_us = 0;
    pulse_record.second_pulse_coming = RT_FALSE;
    pulse_record.second_pulse_timestamp_us = 0;
    pulse_record.state = state;
    pulse_record.rt_isr_pin = rt_pin;
    rt_hw_interrupt_enable(level);

    rt_pin_attach_irq(rt_pin, PIN_IRQ_MODE_RISING_FALLING, pulsein_pin_interrupt_cb, RT_NULL);
    rt_pin_irq_enable(rt_pin, PIN_IRQ_ENABLE); /* enable interrupt */
    rt_err = rt_sem_take(&pulsein_sem, rt_tick_from_millisecond(timeout / 1000));
    rt_pin_irq_enable(rt_pin, PIN_IRQ_DISABLE); /* disable interrupt */

    if (rt_err == RT_EOK)
    {
        level = rt_hw_interrupt_disable();
        delta = (pulse_record.second_pulse_timestamp_us - pulse_record.first_pulse_timestamp_us);
        rt_hw_interrupt_enable(level);
    }
    else
    {
        LOG_E("semaphore error: %d", rt_err);
        delta = 0; /* timeout or other errors */
    }
    return delta;
}

unsigned long pulseInLong(uint8_t pin, uint8_t state, unsigned long timeout)
{
    return pulseIn(pin, state, timeout);
}
