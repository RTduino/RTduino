/*
 * Copyright (c) 2021-2022, RTduino Development Team
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
#include <Arduino.h>
#include "wiring_private.h"
#ifdef PKG_USING_PERF_COUNTER
#include <perf_counter.h>
#endif

#define DBG_TAG    "RTduino.wiring"
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
    rt_device_t hwtimer_device;

    hwtimer_device = rt_device_find(RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME);
    if(hwtimer_device != RT_NULL &&
       rt_device_read(hwtimer_device, 0, &timestamp, sizeof(timestamp)) > 0)
    {
        return timestamp.sec*1000000 + timestamp.usec;
    }
    LOG_E("Failed to read from hardware timer!");
    return 0;
#elif defined(PKG_USING_PERF_COUNTER)
    return get_system_us();
#elif defined(RT_USING_CPUTIME)
    return clock_cpu_microsecond(clock_cpu_gettime());
#else
    LOG_W("Low accuracy micros()");
    return millis() * 1000;
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

#ifdef rt_weak
rt_weak
#else
RT_WEAK
#endif
void yield(void)
{
    rt_schedule();
}
