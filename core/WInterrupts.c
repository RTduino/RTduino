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
 * 2022-04-13     Meco Man     first version
 */

#include <Arduino.h>
#include <rtdevice.h>
#ifdef ARCH_ARM
#include <cmsis_compiler.h>
#else
#include <board.h>
#endif /* ARCH_ARM */

void interrupts(void)
{
#if defined(ARCH_ARM) || defined(ARCH_RISCV)
    __enable_irq();
#else
#warning "Please define interrupts for this architecture in Arduino.h"
#endif /* defined(ARCH_ARM) || defined(ARCH_RISCV) */
}

void noInterrupts(void)
{
#if defined(ARCH_ARM) || defined(ARCH_RISCV)
    __disable_irq();
#else
#warning "Please define noInterrupts for this architecture in Arduino.h"
#endif /* defined(ARCH_ARM) || defined(ARCH_RISCV) */
}

#ifdef RT_USING_PIN
#include "wiring_private.h"

#define DBG_TAG    "RTduino.interrupts"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

static rt_base_t ard2rt_interrupt_mode(uint8_t mode)
{
    rt_base_t rt_mode = -1;

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
    }

    return rt_mode;
}

/**
 * @brief Maps a digital pin to its corresponding interrupt number.
 *
 * This function maps a digital pin number to its corresponding interrupt number.
 * The interrupt number is the same as the pin number of RTduino.
 * e.g. digitalPinToInterrupt(0 or D0) returns 0, digitalPinToInterrupt(1 or D1) returns 1.
 *
 * @param pin The digital pin number.
 * @return The interrupt number corresponding to the pin.
 */
uint8_t digitalPinToInterrupt(uint8_t pin)
{
    RTDUINO_CHECK_PIN_LIMIT_RETURN(pin, pin); /* with return value */
    return pin;
}

/**
 * @brief Attaches an interrupt to a specific pin.
 *
 * This function attaches an interrupt to a specific pin. The interrupt is triggered
 * when the specified mode is detected on the pin. The user-defined function is called
 * when the interrupt is triggered.
 *
 * @param interruptNum The interrupt number to attach the interrupt to. Suggested to used digitalPinToInterrupt() to get the interrupt number.
 * @param userFunc The user-defined function to be called when the interrupt is triggered.
 * @param mode The mode in which the interrupt is triggered (LOW, HIGH, CHANGE, RISING, FALLING).
 */
void attachInterrupt(uint8_t interruptNum, void (*userFunc)(void), int mode)
{
    rt_base_t rt_pin = pin_map_table[interruptNum].rt_pin;
    rt_base_t rt_mode = ard2rt_interrupt_mode(mode);

    RTDUINO_CHECK_PIN_LIMIT_RETURN(interruptNum,); /* without return value */

    if (rt_mode < 0)
    {
        LOG_E("[attachInterrupt] Invalid mode %d", mode);
        return;
    }

    rt_pin_attach_irq(rt_pin, (rt_uint8_t)rt_mode, (void*)userFunc, RT_NULL);
    rt_pin_irq_enable(rt_pin, PIN_IRQ_ENABLE);
}

/**
 * @brief Detaches an interrupt from a specific pin.
 *
 * This function detaches an interrupt from a specific pin. The interrupt will no longer
 * be triggered for the pin.
 *
 * @param interruptNum The interrupt number to detach the interrupt from. Suggested to used digitalPinToInterrupt() to get the interrupt number.
 */
void detachInterrupt(uint8_t interruptNum)
{
    rt_int32_t rt_pin;

    RTDUINO_CHECK_PIN_LIMIT_RETURN(interruptNum,); /* without return value */

    rt_pin = pin_map_table[interruptNum].rt_pin;
    rt_pin_irq_enable(rt_pin, PIN_IRQ_DISABLE);
    rt_pin_detach_irq(rt_pin);
}

#if defined(RTDUINO_USING_UTEST) || defined(RT_UTEST_USING_ALL_CASES)
rt_base_t LLT__ard2rt_interrupt_mode(uint8_t mode)
{
    return ard2rt_interrupt_mode(mode);
}
#endif /* RTDUINO_USING_UTEST || RT_UTEST_USING_ALL_CASES */

#endif /* RT_USING_PIN */
