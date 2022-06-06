/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-10     Meco Man     first version
 */

#include <rtdevice.h>
#include <Arduino.h>
#include "wiring_private.h"

unsigned long millis(void)
{
    return rt_tick_get_millisecond();
}

unsigned long micros(void)
{
#ifdef RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME
    rt_hwtimerval_t timestamp;
    extern rt_device_t arduino_hwtimer_device;

    if(arduino_hwtimer_device != RT_NULL)
    {
        rt_device_read(arduino_hwtimer_device, 0, &timestamp, sizeof(timestamp));
        return timestamp.sec*1000000 + timestamp.usec;
    }
    else
#endif /* RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME */
    {
        return millis()*1000;
    }
}

void delay(unsigned long ms)
{
    rt_thread_mdelay(ms);
}

void delayMicroseconds(unsigned int us)
{
    rt_hw_us_delay(us);
}

RT_WEAK void yield(void)
{
    rt_schedule();
}
