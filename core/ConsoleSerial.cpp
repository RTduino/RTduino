/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-02-11     Meco Man     first version
 */

#include <rtthread.h>
#include "ConsoleSerial.h"

ConsoleSerial::ConsoleSerial(void)
{

}

void ConsoleSerial::begin(uint32_t BaudRate)
{

}

size_t ConsoleSerial::write(uint8_t c)
{
    /*Code to display letter when given the ASCII code for it*/
    rt_kprintf("%c", c);
    return 1;
}

size_t ConsoleSerial::write(const uint8_t *buffer, size_t size)
{
    /*Code to display array of chars when given a pointer to the beginning of the array and a size
        -- this will not end with the null character*/
    return rt_device_write(rt_console_get_device(), 0, buffer, size);
}

ConsoleSerial Serial;
