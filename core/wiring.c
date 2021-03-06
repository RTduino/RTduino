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

#define DBG_TAG    "Arduino.wiring"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

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
        if(rt_device_read(arduino_hwtimer_device, 0, &timestamp, sizeof(timestamp)) > 0)
        {
            return timestamp.sec*1000000 + timestamp.usec;
        }
        else
        {
            LOG_E("Failed to read from hardware timer!");
            return 0;
        }
    }
    else
#elif defined(RT_USING_CPUTIME)
    if(RT_TRUE)
    {
        return clock_cpu_microsecond(clock_cpu_gettime());
    }
    else
#endif /* RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME */
    {
        LOG_W("Low accuracy micros()");
        return millis() * 1000;
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
