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

#ifdef RTDUINO_CMD_SERIAL_USING_FOCUSON
static rt_bool_t rtduino_serial_focuson_mode = RT_FALSE;

struct rt_ringbuffer *rtduino_serial_focuson_ringbuffer = RT_NULL;
rt_mutex_t rtduino_serial_focuson_ringbuffer_mutex;

int rt_kprintf(const char *fmt, ...)
{
    va_list args;
    rt_size_t length;
    rt_device_t _console_device;
    static char rt_log_buf[RT_CONSOLEBUF_SIZE];

    if(rtduino_serial_focuson_mode && rt_thread_self() != rt_thread_find("RTduino"))
    {
        /* only can be printed in RTduino thread in foucs on mode is enabled */
        return RT_NULL;
    }

    va_start(args, fmt);
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

/* pre-init the "serial focuson" command's environment
 * If we don't do the pre-init operation, when the users invoke
 * Serial.read() function, it will be asserted because of mutex is NULL
 */
static int rtduino_cmd_serial_focuson_preinit(void)
{
    rtduino_serial_focuson_ringbuffer_mutex = rt_mutex_create("RTduCOM", RT_IPC_FLAG_PRIO);
    rt_mutex_take(rtduino_serial_focuson_ringbuffer_mutex, RT_WAITING_FOREVER);
    rtduino_serial_focuson_ringbuffer = rt_ringbuffer_create(RT_SERIAL_RB_BUFSZ);
    rt_mutex_release(rtduino_serial_focuson_ringbuffer_mutex);
    return 0;
}
INIT_PREV_EXPORT(rtduino_cmd_serial_focuson_preinit);

void _cmd_serial_focuson(void)
{
    static rt_bool_t init = RT_FALSE;
    rt_size_t getsize;
    rt_uint8_t readchar;

    if(init != RT_TRUE)
    {
        rtduino_serial_focuson_mode = RT_TRUE;
        init = RT_TRUE;
    }

    while(1)
    {
        readchar = (unsigned char)finsh_getchar();
        rt_mutex_take(rtduino_serial_focuson_ringbuffer_mutex, RT_WAITING_FOREVER);
        getsize = rt_ringbuffer_putchar(rtduino_serial_focuson_ringbuffer, readchar);
        rt_mutex_release(rtduino_serial_focuson_ringbuffer_mutex);
        if(getsize != 1)
        {
            //LOG_E("ringbuffer write error!");
        }
    }
}
#endif /* RTDUINO_CMD_SERIAL_USING_FOCUSON */
