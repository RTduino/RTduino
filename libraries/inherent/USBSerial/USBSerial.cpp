/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-02-11     Meco Man     first version
 */

#include <tusb.h>
#include "USBSerial.h"

void USBSerial::begin(uint32_t baud)
{
    begin(baud, SERIAL_8N1);
}

void USBSerial::begin(uint32_t baud, uint8_t config)
{
    /* Nothing need to do */
}

/*Code to display letter when given the ASCII code for it*/
size_t USBSerial::write(uint8_t c)
{
    if (tud_cdc_connected())
    {
        tud_cdc_write_char(c);
        tud_cdc_write_flush();
    }
    return 1;
}

/*Code to display array of chars when given a pointer to the beginning of the array and a size
    -- this will not end with the null character*/
size_t USBSerial::write(const uint8_t *buffer, size_t size)
{
    size_t byte_written;

    if (tud_cdc_connected())
    {
        byte_written = tud_cdc_write(buffer, size);
        tud_cdc_write_flush();
    }
    return byte_written;
}

USBSerial SerialUSB;
