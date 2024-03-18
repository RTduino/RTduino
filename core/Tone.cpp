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
 * 2023-06-25     Meco Man     first version
 */

#include <RTduino.h>

#define DBG_TAG    "RTduino.Tone"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

#ifdef RTDUINO_TONE_HWTIMER_DEVICE_NAME

static volatile uint8_t _tone_pin = RT_NULL;


/**
 * @brief Callback function for tone frequency.
 *
 * This function is called when the tone frequency needs to be updated.
 *
 * @param dev The device associated with the callback.
 * @param size The size of the data (unused in this function).
 * @return The error code indicating the status of the callback.
 */
static rt_err_t _tone_freq_cb(rt_device_t dev, rt_size_t size)
{
    RT_UNUSED(dev);
    RT_UNUSED(size);

    digitalWrite(_tone_pin, !digitalRead(_tone_pin));
    return RT_EOK;
}

static void _tone_duration_cb(void *parameter)
{
    RT_UNUSED(parameter);

    noTone(_tone_pin);
}

void tone(uint8_t pin, unsigned int frequency, unsigned long duration)
{
    rt_hwtimerval_t val; /* callback interval */
    rt_hwtimer_mode_t mode = HWTIMER_MODE_PERIOD; /* periodic */
    rt_uint32_t freq = 1000000; /* 1Mhz, 1us */
    rt_device_t hwtimer_device;
    rt_err_t err;

    hwtimer_device = rt_device_find(RTDUINO_TONE_HWTIMER_DEVICE_NAME);
    if (hwtimer_device == RT_NULL)
    {
        LOG_W("Cannot find a hardware timer: %s!", RTDUINO_TONE_HWTIMER_DEVICE_NAME);
        return;
    }

    err = rt_device_open(hwtimer_device, RT_DEVICE_OFLAG_RDWR);
    if (!(err == RT_EOK || err == -RT_EBUSY))
    {
        LOG_E("Failed to open hardware timer: %s!", RTDUINO_TONE_HWTIMER_DEVICE_NAME);
        return;
    }

    rt_device_control(hwtimer_device, HWTIMER_CTRL_FREQ_SET, &freq); /* set hwtimer prescaler frequency */
    rt_device_control(hwtimer_device, HWTIMER_CTRL_MODE_SET, &mode); /* set hwtimer mode */
    rt_device_set_rx_indicate(hwtimer_device, _tone_freq_cb);

    if (frequency == 0)
    {
        LOG_W("Tone: illegal freqency parameter!");
        return; /* illegal parameter */
    }
    else if (frequency == 1)
    {
        val.sec = 1;
        val.usec = 0;
    }
    else
    {
        val.sec = 0;
        val.usec = 1000000 / (frequency * 2); /* 50% duty, so double frequency */
    }

    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    _tone_pin = pin;

    if(rt_device_write(hwtimer_device, 0, &val, sizeof(val)) == 0)
    {
        LOG_E("Failed to start (write) hardware timer: %s!", RTDUINO_TONE_HWTIMER_DEVICE_NAME);
    }
    else if (duration != 0)
    {
        static rt_timer_t _tone_duration_timer = RT_NULL;

        if (_tone_duration_timer == RT_NULL)
        {
            _tone_duration_timer = rt_timer_create("tone", _tone_duration_cb, RT_NULL,
                                        rt_tick_from_millisecond(duration),
                                        RT_TIMER_FLAG_ONE_SHOT | RT_TIMER_FLAG_SOFT_TIMER);
        }

        if (_tone_duration_timer != RT_NULL)
        {
            rt_timer_start(_tone_duration_timer);
        }
    }
}

void noTone(uint8_t pin)
{
    RT_UNUSED(pin);

    rt_device_t hwtimer_device = rt_device_find(RTDUINO_TONE_HWTIMER_DEVICE_NAME);
    if (hwtimer_device)
    {
        rt_device_close(hwtimer_device);
    }
}
#else
rt_weak void tone(uint8_t pin, unsigned int frequency, unsigned long duration)
{
    RT_UNUSED(pin);
    RT_UNUSED(frequency);
    RT_UNUSED(duration);

    LOG_E("tone() is not yet implemented, please define RTDUINO_TONE_HWTIMER_DEVICE_NAME in pins_arduino.h!");
}

rt_weak void noTone(uint8_t pin)
{
    RT_UNUSED(pin);
    LOG_E("noTone() is not yet implemented, please define RTDUINO_TONE_HWTIMER_DEVICE_NAME in pins_arduino.h!");
}
#endif /* RTDUINO_TONE_HWTIMER_DEVICE_NAME */
