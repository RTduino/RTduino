/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-10     Meco Man     first version
 */

#include <rtdevice.h>
#include <Arduino.h>
#include "wiring_private.h"
#ifdef PKG_USING_PERF_COUNTER
#include <perf_counter.h>
#endif

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
            goto _exit;
        }
    }

_exit:
    LOG_W("Low accuracy micros()");
    return millis() * 1000;
#elif defined(PKG_USING_PERF_COUNTER)
    return get_system_us();
#elif defined(RT_USING_CPUTIME)
    return clock_cpu_microsecond(clock_cpu_gettime());
#endif /* RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME */
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
