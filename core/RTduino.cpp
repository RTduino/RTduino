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
 * 2022-05-01     Meco Man     support hardware timer
 * 2022-05-20     Meco Man     support no setup-loop structure
 * 2023-06-22     Meco Man     add auto-init macros and functions
 */

#include <RTduino.h>
#include "wiring_private.h"

#define DBG_TAG    "RTduino.thread"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

/**
 * @brief Entry point for the sketch loader thread.
 *
 * This function is the entry point for the sketch loader thread.
 * It calls the setup function once, and then enters an infinite loop calling the loop function repeatedly.
 *
 * @param parameter A pointer to the sketch loader.
 */
static void rtduino_sketch_entry(void *parameter)
{
    rtduino_loader_t loader = (rtduino_loader_t)parameter;
    loader->setup();
    while (1)
    {
        loader->loop();
    }
}

rtduino_loader_t rtduino_sketch_loader_create_stacksize_prio(const char* name,
        void (*setup)(void), void (*loop)(void), rt_uint32_t stack_size, rt_uint8_t priority)
{
    rtduino_loader_t loader;

    if (name == RT_NULL || setup == RT_NULL || loop == RT_NULL)
    {
        return RT_NULL;
    }

    loader = (rtduino_loader_t)rt_malloc(sizeof(rtduino_loader));
    if (loader == RT_NULL)
    {
        return RT_NULL;
    }

    loader->name =  name;
    loader->setup = setup;
    loader->loop = loop;
    loader->thread = RT_NULL;

    rt_thread_t thread = rt_thread_create(name, rtduino_sketch_entry,
                (void *)loader, stack_size, priority, 20);
    if (thread != RT_NULL)
    {
        loader->thread = thread;
        rt_thread_startup(thread);
    }
    else
    {
        rt_free(loader);
        loader = RT_NULL;
    }

    return loader;
}

rt_err_t rtduino_sketch_loader_delete(rtduino_loader_t loader)
{
    rt_err_t err;

    if (loader == RT_NULL)
    {
        return -RT_EINVAL;
    }

    err = rt_thread_delete(loader->thread);
    if (err == RT_EOK)
    {
        rt_free(loader);
    }

    return err;
}

#ifdef RTDUINO_1US_HWTIMER_DEVICE_NAME
/**
 * @brief Initializes the hardware timer used for the `macros()` function, if available.
 *
 * This hardware timer is to support the macros() function to deliver a
 * 1us level counter. Normally, there should be some other option will be
 * used. This timer just a alternative feature. If there is no other functions
 * can be used. This hardware timer will be used.
 */
static void hwtimer_1us_init(void)
{
    rt_device_t hwtimer_device;

    hwtimer_device = rt_device_find(RTDUINO_1US_HWTIMER_DEVICE_NAME);
    if (hwtimer_device != RT_NULL)
    {
        rt_hwtimer_mode_t mode = HWTIMER_MODE_PERIOD; /* periodic */
        rt_uint32_t freq = 1000000; /* 1Mhz, 1us */
        rt_hwtimerval_t val = {.sec=1, .usec=0}; /* callback interval */

        if (rt_device_open(hwtimer_device, RT_DEVICE_OFLAG_RDWR) != RT_EOK)
        {
            LOG_E("Failed to open hardware timer: %s!", RTDUINO_1US_HWTIMER_DEVICE_NAME);
            return;
        }
        rt_device_control(hwtimer_device, HWTIMER_CTRL_FREQ_SET, &freq); /* set hwtimer prescaler frequency */
        rt_device_control(hwtimer_device, HWTIMER_CTRL_MODE_SET, &mode); /* set hwtimer mode */
        if (rt_device_write(hwtimer_device, 0, &val, sizeof(val)) == 0)
        {
            LOG_E("Failed to start (write) hardware timer: %s!", RTDUINO_1US_HWTIMER_DEVICE_NAME);
        }
    }
    else
    {
        LOG_W("Cannot find a hardware timer: %s!", RTDUINO_1US_HWTIMER_DEVICE_NAME);
    }
}
#endif /* RTDUINO_1US_HWTIMER_DEVICE_NAME */

/**
 * @brief Initializes the BSP variant.
 *
 * This function is called during library initialization to initialize the BSP variant.
 * It can be left blank if no initialization is required.
 */
rt_weak void initVariant(void)
{
    /* maybe a blank function and does nothing */
}

/**
 * @brief Initializes the RTduino.
 *
 * This function is called during RT-Thread initialization to initialize the RTduino library.
 * It initializes:
 *  - the hardware timer (if available)
 *  - calls the `initVariant()` function to initialize the BSP variant
 *  - creates the sketch loader
 *
 * @return RT_EOK if the initialization is successful.
 */
static int rtduino_init(void)
{
#ifdef RTDUINO_1US_HWTIMER_DEVICE_NAME
    hwtimer_1us_init();
#endif /* RTDUINO_1US_HWTIMER_DEVICE_NAME */

    initVariant();

#ifndef RTDUINO_NO_SETUP_LOOP
    rtduino_sketch_loader_create("RTduino", setup, loop);
#endif /* RTDUINO_NO_SETUP_LOOP */
    return 0;
}
INIT_COMPONENT_EXPORT(rtduino_init);
