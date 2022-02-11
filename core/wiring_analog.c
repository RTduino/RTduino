/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-10     Meco Man     first version
 */

#include <Arduino.h>
#include "wiring_private.h"
#include <rtdevice.h>

#define PWM_PERIOD_NS 1000*1000*1000/*ns*//ARDUINO_PWM_HZ

void analogWrite(uint8_t pin, int val)
{
    struct rt_device_pwm *pwm_dev;
    rt_uint32_t rt_val;

    pwm_dev = (struct rt_device_pwm*)rt_device_find(pin_map_table[pin].device_name);
    if(pwm_dev != RT_NULL)
    {
        rt_val = map(val, 0, 255, 0, PWM_PERIOD_NS);
        rt_pwm_disable(pwm_dev, pin_map_table[pin].channel);
        rt_pwm_set(pwm_dev, pin_map_table[pin].channel, PWM_PERIOD_NS, rt_val);
        rt_pwm_enable(pwm_dev, pin_map_table[pin].channel);
    }
}

int analogRead(uint8_t pin)
{
    rt_adc_device_t adc_dev;

    adc_dev = (rt_adc_device_t)rt_device_find(pin_map_table[pin].device_name);
    if(adc_dev != RT_NULL)
    {
        rt_adc_enable(adc_dev, pin_map_table[pin].channel);
        return rt_adc_read(adc_dev, pin_map_table[pin].channel);
    }
    else
    {
        return 0;
    }
}
