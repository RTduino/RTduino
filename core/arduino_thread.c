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
#include <rtthread.h>

#define DBG_TAG    "Arduino"
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

/* put your setup code here, to run once: */
RT_WEAK void setup(void)
{
    LOG_W("run in weak setup(): %s", __FILE__);
}

/* put your main code here, to run repeatedly: */
RT_WEAK void loop(void)
{
    LOG_W("run in weak loop(): %s", __FILE__);
}

static void arduino_entry(void *parameter)
{
    initVariant();
    setup();

    while(1)
    {
        loop();
        rt_schedule();
    }
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
INIT_APP_EXPORT(arduino_thread_init);
