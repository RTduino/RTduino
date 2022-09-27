/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-09-26     Meco Man     first version
 */

#include <rtdevice.h>
#include <shell.h>

#define DBG_TAG    "Arduino.cmd"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

struct rt_ringbuffer *console_serial_ringbuffer = RT_NULL;

rt_mutex_t arduino_serial_ringbuffer_mutex;

void _cmd_serial_foucson(void)
{
    static rt_bool_t init = RT_FALSE;
    rt_size_t getsize;
    rt_uint8_t readchar;

    if(init != RT_TRUE)
    {
        arduino_serial_ringbuffer_mutex =
                rt_mutex_create("arduino", RT_IPC_FLAG_PRIO);
        init = RT_TRUE;
    }

    rt_mutex_take(arduino_serial_ringbuffer_mutex, RT_WAITING_FOREVER);
    console_serial_ringbuffer = rt_ringbuffer_create(RT_SERIAL_RB_BUFSZ);
    rt_mutex_release(arduino_serial_ringbuffer_mutex);
    while(1)
    {
        readchar = (unsigned char)finsh_getchar();
        rt_mutex_take(arduino_serial_ringbuffer_mutex, RT_WAITING_FOREVER);
        getsize = rt_ringbuffer_putchar(console_serial_ringbuffer, readchar);
        rt_mutex_release(arduino_serial_ringbuffer_mutex);
        if(getsize != 1)
        {
            LOG_E("ringbuffer write error!");
        }
    }
    rt_ringbuffer_destroy(console_serial_ringbuffer);
}
