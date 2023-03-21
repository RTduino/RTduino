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

#include <Arduino.h>
#include <rtthread.h>

#ifdef RTDUINO_CMD_SERIAL_USING_FOCUSON

extern rt_mutex_t rtduino_serial_focuson_ringbuffer_mutex;
extern struct rt_ringbuffer *rtduino_serial_focuson_ringbuffer;

int ConsoleSerial::available(void)
{
    int len;
    rt_mutex_take(rtduino_serial_focuson_ringbuffer_mutex, RT_WAITING_FOREVER);
    len = rt_ringbuffer_data_len(rtduino_serial_focuson_ringbuffer);
    rt_mutex_release(rtduino_serial_focuson_ringbuffer_mutex);
    return len;
}

int ConsoleSerial::peek(void)
{
    rt_uint8_t ch, *p_ch;
    if(this->available()<=0)
    {
        return -1;
    }
    rt_mutex_take(rtduino_serial_focuson_ringbuffer_mutex, RT_WAITING_FOREVER);
    rt_ringbuffer_peek(rtduino_serial_focuson_ringbuffer, &p_ch);
    ch = *p_ch;
    rt_mutex_release(rtduino_serial_focuson_ringbuffer_mutex);
    return (int)ch;
}

int ConsoleSerial::read(void)
{
    rt_uint8_t ch;
    if(this->available()<=0)
    {
        return -1;
    }
    rt_mutex_take(rtduino_serial_focuson_ringbuffer_mutex, RT_WAITING_FOREVER);
    rt_ringbuffer_getchar(rtduino_serial_focuson_ringbuffer, &ch);
    rt_mutex_release(rtduino_serial_focuson_ringbuffer_mutex);
    return (int)ch;
}

#endif /* RTDUINO_CMD_SERIAL_USING_FOCUSON */
