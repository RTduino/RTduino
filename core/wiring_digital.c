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

void pinMode(uint8_t pin, uint8_t mode)
{
    rt_base_t rt_mode = RT_NULL;

    RT_ASSERT(mode == INPUT || mode == OUTPUT ||
              mode == INPUT_PULLUP || mode == INPUT_PULLDOWN ||
              mode == OUTPUT_OPEN_DRAIN);

    RTDUINO_CHECK_PIN_LIMIT_RETURN(pin,); /* without return value */

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

    default:
        LOG_E("[pinMode] Invalid mode %u", mode);
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
    rt_base_t rt_val = RT_NULL;

    RT_ASSERT(val == HIGH || val == LOW);

    RTDUINO_CHECK_PIN_LIMIT_RETURN(pin,); /* without return value */

    if(val == HIGH)
    {
        rt_val = PIN_HIGH;
    }
    else if(val == LOW)
    {
        rt_val = PIN_LOW;
    }

    rt_pin_write(pin_map_table[pin].rt_pin, rt_val);
}

int digitalRead(uint8_t pin)
{
    RTDUINO_CHECK_PIN_LIMIT_RETURN(pin, LOW); /* with return value */
    return (rt_pin_read(pin_map_table[pin].rt_pin) == PIN_HIGH)? HIGH : LOW;
}

#endif /* RT_USING_PIN */
