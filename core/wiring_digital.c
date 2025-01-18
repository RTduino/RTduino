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
 * 2021-12-10     Meco Man     first version
 */

#include <rtdevice.h>

#ifdef RT_USING_PIN
#include <Arduino.h>
#include "wiring_private.h"

#define DBG_TAG    "RTduino.digital"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

static rt_base_t ard2rt_pin_mode(uint8_t mode)
{
    rt_base_t rt_mode = -1;

    switch(mode)
    {
    case INPUT:
        rt_mode = PIN_MODE_INPUT;
        break;

    case OUTPUT:
        rt_mode = PIN_MODE_OUTPUT;
        break;

    case INPUT_PULLUP:
        rt_mode = PIN_MODE_INPUT_PULLUP;
        break;

    case INPUT_PULLDOWN:
        rt_mode = PIN_MODE_INPUT_PULLDOWN;
        break;

    case OUTPUT_OPEN_DRAIN:
        rt_mode = PIN_MODE_OUTPUT_OD;
        break;
    }

    return rt_mode;
}

static rt_base_t ard2rt_pin_level(uint8_t val)
{
    rt_base_t rt_val = -1;

    switch(val)
    {
    case HIGH:
        rt_val = PIN_HIGH;
        break;
    case LOW:
        rt_val = PIN_LOW;
        break;
    }

    return rt_val;
}

void pinMode(uint8_t pin, uint8_t mode)
{
    RTDUINO_CHECK_PIN_LIMIT_RETURN(pin,); /* without return value */

    rt_base_t rt_mode = ard2rt_pin_mode(mode);
    if (rt_mode < 0)
    {
        LOG_E("[pinMode] Invalid mode %d", mode);
        return;
    }

    if (pin_map_table[pin].device_name != RT_NULL)
    {
        LOG_I("[pinMode] Pin %d was a %s device IO, but now it becomes a general-purpose IO.", pin, pin_map_table[pin].device_name);
    }
    rt_pin_mode(pin_map_table[pin].rt_pin, rt_mode);
}

void digitalWrite(uint8_t pin, uint8_t val)
{
    RTDUINO_CHECK_PIN_LIMIT_RETURN(pin,); /* without return value */

    rt_base_t rt_val = ard2rt_pin_level(val);
    if (rt_val < 0)
    {
        LOG_E("[digitalWrite] Invalid value %d", val);
        return;
    }
    rt_pin_write(pin_map_table[pin].rt_pin, rt_val);
}

int digitalRead(uint8_t pin)
{
    RTDUINO_CHECK_PIN_LIMIT_RETURN(pin, LOW); /* with return value */
    return (rt_pin_read(pin_map_table[pin].rt_pin) == PIN_HIGH)? HIGH : LOW;
}

#if defined(RTDUINO_USING_UTEST) || defined(RT_UTEST_USING_ALL_CASES)
rt_base_t LLT__ard2rt_pin_mode(uint8_t mode)
{
    return ard2rt_pin_mode(mode);
}

rt_base_t LLT__ard2rt_pin_level(uint8_t mode)
{
    return ard2rt_pin_level(mode);
}
#endif /* defined(RTDUINO_USING_UTEST) || defined(RT_UTEST_USING_ALL_CASES) */

#endif /* RT_USING_PIN */
