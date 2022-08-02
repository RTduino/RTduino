/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-13     Meco Man     first version
 */

#include <rtdevice.h>

#ifdef RT_USING_PIN
#include "wiring_private.h"

#define DBG_TAG    "Arduino.interrupts"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

/* for RT-Thread: Arduino interruptNum = Arduino pin number */

uint8_t digitalPinToInterrupt(uint8_t pin)
{
    return pin;
}

void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode)
{
    rt_int32_t rt_pin;
    rt_uint32_t rt_mode;

    rt_pin = pin_map_table[interruptNum].rt_pin;

    switch(mode)
    {
    case LOW:
        rt_mode = PIN_IRQ_MODE_LOW_LEVEL;
        break;

    case HIGH:
        rt_mode = PIN_IRQ_MODE_HIGH_LEVEL;
        break;

    case CHANGE:
        rt_mode = PIN_IRQ_MODE_RISING_FALLING;
        break;

    case RISING:
        rt_mode = PIN_IRQ_MODE_RISING;
        break;

    case FALLING:
        rt_mode = PIN_IRQ_MODE_FALLING;
        break;

    default:
        rt_mode = RT_NULL;
        LOG_E("attachInterrupt mode parameter is illegal");
        break;
    }

    rt_pin_attach_irq(rt_pin, rt_mode, (void*)userFunc, RT_NULL);
    rt_pin_irq_enable(rt_pin, PIN_IRQ_ENABLE);
}

void detachInterrupt(uint8_t interruptNum)
{
    rt_int32_t rt_pin;

    rt_pin = pin_map_table[interruptNum].rt_pin;
    rt_pin_irq_enable(rt_pin, PIN_IRQ_DISABLE);
    rt_pin_detach_irq(rt_pin);
}

#endif /* RT_USING_PIN */
