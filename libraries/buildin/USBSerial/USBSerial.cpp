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
 * 2022-03-24     Meco Man     first version
 * 2022-06-03     Meco Man     add read features
 */

#include <tusb.h>
#include <Arduino.h>
#include "USBSerial.h"

void USBSerial::begin(void)
{
    /* Do nothing. Use RT-Thread default configurations: typically 115200, 8N1 */
}

void USBSerial::begin(uint32_t baud)
{
    /* TODO */
}

void USBSerial::begin(uint32_t baud, uint8_t config)
{
    /* TODO */
}

void USBSerial::end(void)
{
    // Nothing to do.
    // RTduino has not right to close RT-Thread devices.
    // This could cause other threads make errors.
}

/*Code to display letter when given the ASCII code for it*/
size_t USBSerial::write(uint8_t ch)
{
    return write(&ch, 1);
}

/*Code to display array of chars when given a pointer to the beginning of the array and a size
    -- this will not end with the null character*/
size_t USBSerial::write(const uint8_t *buffer, size_t size)
{
    size_t remain = size;

    while (remain && tud_cdc_connected())
    {
        size_t wrcount = tud_cdc_write(buffer, remain);
        remain -= wrcount;
        buffer += wrcount;
        tud_cdc_write_flush();
        // Write FIFO is full, run usb background to flush
        if (remain)
        {
            yield();
        }
    }
    return size - remain;
}

int USBSerial::available(void)
{
    return tud_cdc_available();
}

int USBSerial::peek(void)
{
    uint8_t ch;
    return tud_cdc_peek(&ch) ? (int)ch : -1;
}

int USBSerial::read(void)
{
    return (int)tud_cdc_read_char();
}

size_t USBSerial::read(uint8_t *buffer, size_t size)
{
    return tud_cdc_read(buffer, size);
}

int USBSerial::availableForWrite(void)
{
    return tud_cdc_write_available();
}

void USBSerial::flush(void)
{
    tud_cdc_write_flush();
}

USBSerial SerialUSB;
