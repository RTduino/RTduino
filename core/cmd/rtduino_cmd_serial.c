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

#define DBG_TAG    "RTduino.cmd"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

static rt_bool_t rtduino_serial_focuson_mode = RT_FALSE;

int rt_kprintf(const char *fmt, ...)
{
    va_list args;
    rt_size_t length;
    rt_device_t _console_device;
    static char rt_log_buf[RT_CONSOLEBUF_SIZE];

    if(rtduino_serial_focuson_mode && rt_thread_self() != rt_thread_find("Arduino"))
    {
        /* only can be printed in Arduino thread in foucs on mode is enabled */
        return RT_NULL;
    }

    va_start(args, fmt);
    /* the return value of vsnprintf is the number of bytes that would be
     * written to buffer had if the size of the buffer been sufficiently
     * large excluding the terminating null byte. If the output string
     * would be larger than the rt_log_buf, we have to adjust the output
     * length. */
    length = rt_vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
    if (length > RT_CONSOLEBUF_SIZE - 1)
        length = RT_CONSOLEBUF_SIZE - 1;
    _console_device = rt_console_get_device();
    if (_console_device != RT_NULL)
    {
        rt_device_write(_console_device, 0, rt_log_buf, length);
    }
    va_end(args);

    return length;
}

struct rt_ringbuffer *console_serial_ringbuffer = RT_NULL;

rt_mutex_t rtduino_serial_ringbuffer_mutex;

void _cmd_serial_foucson(void)
{
    static rt_bool_t init = RT_FALSE;
    rt_size_t getsize;
    rt_uint8_t readchar;

    if(init != RT_TRUE)
    {
        rtduino_serial_ringbuffer_mutex =
                rt_mutex_create("RTduCOM", RT_IPC_FLAG_PRIO);
        rtduino_serial_focuson_mode = RT_TRUE;
        init = RT_TRUE;
    }

    rt_mutex_take(rtduino_serial_ringbuffer_mutex, RT_WAITING_FOREVER);
    console_serial_ringbuffer = rt_ringbuffer_create(RT_SERIAL_RB_BUFSZ);
    rt_mutex_release(rtduino_serial_ringbuffer_mutex);
    while(1)
    {
        readchar = (unsigned char)finsh_getchar();
        rt_mutex_take(rtduino_serial_ringbuffer_mutex, RT_WAITING_FOREVER);
        getsize = rt_ringbuffer_putchar(console_serial_ringbuffer, readchar);
        rt_mutex_release(rtduino_serial_ringbuffer_mutex);
        if(getsize != 1)
        {
            LOG_E("ringbuffer write error!");
        }
    }
    rt_ringbuffer_destroy(console_serial_ringbuffer);
}
