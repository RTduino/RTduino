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

#define DBG_TAG    "Arduino"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

#define PWM_PERIOD_NS 1000*1000*1000/*ns*//ARDUINO_PWM_HZ

void analogWrite(uint8_t pin, int val)
{
#ifdef RT_USING_PWM
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
    else
#endif /* RT_USING_PWM */
    {
        if (val < 128)
        {
            digitalWrite(pin, LOW);
        }
        else
        {
            digitalWrite(pin, HIGH);
        }
        LOG_W("This pin %d doesn't support PWM."
              "Values less than 128 is LOW and higher values is HIGH", pin);
    }
}

void analogReference(uint8_t mode)
{
    LOG_E("analogReference() has not been implemented yet!");
}


int analogRead(uint8_t pin)
{
#ifdef RT_USING_ADC
    rt_adc_device_t adc_dev;

    adc_dev = (rt_adc_device_t)rt_device_find(pin_map_table[pin].device_name);
    if(adc_dev != RT_NULL)
    {
        rt_adc_enable(adc_dev, pin_map_table[pin].channel);
        return rt_adc_read(adc_dev, pin_map_table[pin].channel);
    }
    else
#endif /* RT_USING_ADC */
    {
        LOG_E("This pin %d doesn't support ADC", pin);
        return 0;
    }
}
