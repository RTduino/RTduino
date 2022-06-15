/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-10     Meco Man     first version
 * 2022-05-01     Meco Man     support hardware timer
 * 2022-05-20     Meco Man     support no setup-loop structure
 */

#include <Arduino.h>
#include <rtdevice.h>
#include "wiring_private.h"

#define DBG_TAG    "Arduino.thread"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

#ifndef RTDUINO_THREAD_SIZE
#define RTDUINO_THREAD_SIZE     1024
#endif /* RTDUINO_THREAD_SIZE */

#ifndef RTDUINO_THREAD_PRIO
#define RTDUINO_THREAD_PRIO     30
#endif /* RTDUINO_THREAD_PRIO */

/* initialization for BSP; maybe a blank function  */
RT_WEAK void initVariant(void)
{
}

#ifdef RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME
rt_device_t arduino_hwtimer_device = RT_NULL;

static void hwtimer_init(void)
{
    rt_device_t hwtimer_device;

    hwtimer_device = rt_device_find(RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME);
    if(hwtimer_device != RT_NULL)
    {
        rt_hwtimer_mode_t mode = HWTIMER_MODE_PERIOD; /* periodic */
        rt_uint32_t freq = 1000000; /* 1Mhz, 1us */
        rt_hwtimerval_t val = {.sec=1, .usec=0}; /* callback interval */

        if(rt_device_open(hwtimer_device, RT_DEVICE_OFLAG_RDWR) != RT_EOK)
        {
            LOG_E("Failed to open hardware timer!");
            return;
        }
        rt_device_control(hwtimer_device, HWTIMER_CTRL_FREQ_SET, &freq); /* set hwtimer prescaler frequency */
        rt_device_control(hwtimer_device, HWTIMER_CTRL_MODE_SET, &mode); /* set hwtimer mode */
        if(rt_device_write(hwtimer_device, 0, &val, sizeof(val)) != 0)
        {
            arduino_hwtimer_device = hwtimer_device;
        }
        else
        {
            LOG_E("Failed to start (write) hardware timer!");
        }
    }
    else
    {
        LOG_W("Cannot find a hardware timer. Some functions cannot be used.");
    }
}
#endif /* RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME */

static void arduino_entry(void *parameter)
{
#ifdef RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME
    hwtimer_init();
#endif /* RTDUINO_DEFAULT_HWTIMER_DEVICE_NAME */
    initVariant();
#ifndef RTDUINO_NO_SETUP_LOOP
    setup();

    while(1)
    {
        loop();
        rt_schedule();
    }
#endif /* RTDUINO_NO_SETUP_LOOP */
}

static int arduino_thread_init(void)
{
    rt_thread_t tid;
    tid = rt_thread_create("Arduino",
                            arduino_entry, RT_NULL,
                            RTDUINO_THREAD_SIZE,
                            RTDUINO_THREAD_PRIO, 20);

    if (tid != RT_NULL)
    {
        rt_thread_startup(tid);
    }
    else
    {
        LOG_E("Arduino thread initialization failed!");
    }

    return 0;
}
INIT_COMPONENT_EXPORT(arduino_thread_init);


#ifdef RT_USING_FINSH
#include <string.h>
#include <finsh.h>
#include <shell.h>

static void _cmd_serial_read(void)
{
    while(1)
    {
        rt_kprintf("%c",(char)finsh_getchar());
    }
}

static int _cmd_arduino(int argc, char **argv)
{
    if(strcmp(argv[1], "serial") == 0)
    {
        _cmd_serial_read();
    }
    return 0;
}
MSH_CMD_EXPORT_ALIAS(_cmd_arduino, arduino, arduino [option]);
#endif /* RT_USING_FINSH */
