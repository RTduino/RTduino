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
 * 2023-06-24     Meco Man     add rt_weak for micros()
 */

#include <RTduino.h>
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

rt_weak unsigned long micros(void)
{
#if defined(RTDUINO_1US_HWTIMER_DEVICE_NAME)
    rt_hwtimerval_t timestamp;
    rt_device_t hwtimer_device;

    hwtimer_device = rt_device_find(RTDUINO_1US_HWTIMER_DEVICE_NAME);
    if(hwtimer_device != RT_NULL &&
       rt_device_read(hwtimer_device, 0, &timestamp, sizeof(timestamp)) > 0)
    {
        return timestamp.sec*1000000 + timestamp.usec;
    }
    LOG_E("Failed to read from hardware timer %s!", RTDUINO_1US_HWTIMER_DEVICE_NAME);
    return 0;
#elif defined(PKG_USING_PERF_COUNTER)
    return get_system_us();
#elif defined(RT_USING_CPUTIME)
    return clock_cpu_microsecond(clock_cpu_gettime());
#else
    static rt_bool_t _low_accuracy_micros_warned = RT_FALSE;
    if (_low_accuracy_micros_warned == RT_FALSE)
    {
        LOG_W("Low accuracy micros()");
        _low_accuracy_micros_warned= RT_TRUE;
    }
    return millis() * 1000;
#endif
}

void delay(unsigned long ms)
{
    rt_thread_mdelay(ms);
}

void delayMicroseconds(unsigned int us)
{
    rt_hw_us_delay(us);
}

rt_weak void yield(void)
{
    rt_schedule();
}
