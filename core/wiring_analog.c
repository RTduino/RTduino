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

#include <Arduino.h>
#include "wiring_private.h"
#include <rtdevice.h>

#define DBG_TAG    "RTduino.analog"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

static uint8_t _analog_read_resolution = 10; /* default: Arduino UNO is 10 bits */
static uint8_t _analog_write_resolution = 8; /* default: Arduino UNO is 8 bits */
static uint32_t _analog_pwm_hz = 500; /* default: Arduino UNO's PWM is around 500Hz */

#define PWM_PERIOD_NS (1000*1000*1000/*ns*//_analog_pwm_hz)

/**
 * @brief Set the reference voltage for analog input.
 *
 * This function is not yet implemented.
 *
 * @param mode The reference voltage mode.
 */
void analogReference(uint8_t mode)
{
    RT_UNUSED(mode);

    LOG_E("analogReference() is not yet implemented!");
}

/**
 * @brief Set the resolution (number of bits) for analogRead() function.
 *
 * @param bits The resolution in bits.
 */
void analogReadResolution(uint8_t bits)
{
    _analog_read_resolution = bits;
}

/**
 * @brief Set the resolution (number of bits) for analogWrite() function.
 *
 * @param bits The resolution in bits.
 */
void analogWriteResolution(uint8_t bits)
{
    _analog_write_resolution = bits;
}

/**
 * @brief Set the PWM frequency (in Hz) for analogWrite() function.
 *
 * @param frequency The frequency in Hz.
 */
void analogWriteFrequency(uint32_t frequency)
{
    _analog_pwm_hz = frequency;
}

/**
 * @brief Write an analog value to a PWM or DAC pin.
 *
 * This function writes an analog value to the specified pin.
 * If the pin supports PWM, the analog value is converted to a PWM duty cycle and applied to the pin.
 * If the pin supports DAC, the analog value is converted to a DAC value and applied to the pin.
 * If the pin does not support PWM or DAC, the function falls back to digital write.
 *
 * @param pin The pin number.
 * @param val The analog value to write.
 */
void analogWrite(uint8_t pin, int val)
{
    RTDUINO_CHECK_PIN_LIMIT_RETURN(pin,); /* without return value */

#ifdef RT_USING_PWM
    struct rt_device_pwm *pwm_dev;
    rt_uint32_t rt_pwm_val;

    pwm_dev = (struct rt_device_pwm*)rt_device_find(pin_map_table[pin].device_name);
    if(pwm_dev != RT_NULL && pwm_dev->parent.type == RT_Device_Class_PWM)
    {
        rt_pwm_val = map(val, 0, bit(_analog_write_resolution)-1, 0, PWM_PERIOD_NS);
        rt_pwm_disable(pwm_dev, pin_map_table[pin].channel);
        rt_pwm_set(pwm_dev, pin_map_table[pin].channel, PWM_PERIOD_NS, rt_pwm_val);
        rt_pwm_enable(pwm_dev, pin_map_table[pin].channel);
        return;
    }
#endif /* RT_USING_PWM */

#ifdef RT_USING_DAC
    rt_dac_device_t dac_dev;
    rt_uint32_t rt_dac_val;
    rt_uint8_t resolution;

    dac_dev = (rt_dac_device_t)rt_device_find(pin_map_table[pin].device_name);
    if(dac_dev != RT_NULL && dac_dev->parent.type == RT_Device_Class_DAC)
    {
        if(rt_device_control((rt_device_t)dac_dev, RT_DAC_CMD_GET_RESOLUTION, &resolution) != RT_EOK)
        {
            LOG_W("This board doesn't support to adjust DAC resolution.");
            resolution = 12; /* assume the hardware resolution is 12 bits */
        }
        rt_dac_val = map(val, 0, bit(_analog_write_resolution)-1, 0, bit(resolution)-1);
        rt_dac_enable(dac_dev, pin_map_table[pin].channel);
        rt_dac_write(dac_dev, pin_map_table[pin].channel, rt_dac_val);
        return;
    }
#endif /* RT_USING_DAC */

    /* This pin doesn't support PWM or DAC */
    pinMode(pin, OUTPUT);
    if (val < (int)(bit(_analog_write_resolution) / 2))
    {
        digitalWrite(pin, LOW);
    }
    else
    {
        digitalWrite(pin, HIGH);
    }
}

/**
 * @brief Read an analog value from an ADC pin.
 *
 * This function reads an analog value from the specified pin.
 * If the pin supports ADC, the analog value is read from the pin.
 * If the pin does not support ADC, an error message is logged and 0 is returned.
 *
 * @param pin The pin number.
 * @return The analog value read from the pin. If the pin does not support ADC, 0 is returned.
 */
int analogRead(uint8_t pin)
{
    RTDUINO_CHECK_PIN_LIMIT_RETURN(pin, 0); /* with return value */

#ifdef RT_USING_ADC
    rt_adc_device_t adc_dev;
    rt_uint32_t rt_adc_val;
    rt_uint8_t resolution;

    adc_dev = (rt_adc_device_t)rt_device_find(pin_map_table[pin].device_name);
    if(adc_dev != RT_NULL && adc_dev->parent.type == RT_Device_Class_ADC)
    {
        rt_adc_enable(adc_dev, pin_map_table[pin].channel);
        rt_adc_val = rt_adc_read(adc_dev, pin_map_table[pin].channel);
        rt_adc_disable(adc_dev, pin_map_table[pin].channel);
        if(rt_device_control((rt_device_t)adc_dev, RT_ADC_CMD_GET_RESOLUTION, &resolution) == RT_EOK)
        {
            return map(rt_adc_val, 0, bit(resolution)-1, 0, bit(_analog_read_resolution)-1);
        }
        else
        {
            LOG_W("This board doesn't support to adjust ADC resolution. It will return real ADC value!");
            return rt_adc_val;
        }
    }
    else
#endif /* RT_USING_ADC */
    {
        LOG_E("This pin %d doesn't support ADC", pin);
        return 0;
    }
}
