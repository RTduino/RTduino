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
 * 2022-09-26     Meco Man     first version
 */

#include <rtdevice.h>
#include <shell.h>

#ifdef RTDUINO_CMD_SERIAL_USING_FOCUSON
struct rt_ringbuffer *rtduino_serial_focuson_ringbuffer = RT_NULL;
rt_mutex_t rtduino_serial_focuson_ringbuffer_mutex;

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
    rt_size_t getsize;
    rt_uint8_t readchar;

    rt_device_register(rt_device_create(RT_NULL, RT_NULL), "dummy", RT_NULL);
    rt_console_set_device("dummy"); /* terminate console, kill rt_kprintf */

    while(1)
    {
        /* get a char from shell thread and transfer to rtduino thread */
        readchar = (unsigned char)finsh_getchar();
        rt_mutex_take(rtduino_serial_focuson_ringbuffer_mutex, RT_WAITING_FOREVER);
        rt_ringbuffer_putchar(rtduino_serial_focuson_ringbuffer, readchar);
        rt_mutex_release(rtduino_serial_focuson_ringbuffer_mutex);
    }
}
#endif /* RTDUINO_CMD_SERIAL_USING_FOCUSON */
