/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-02-11     Meco Man     first version
 */

#ifndef USBSerial_h
#define USBSerial_h

#include <HardwareSerial.h>

class USBSerial: public HardwareSerial
{
public:
    void begin(uint32_t baud);
    void begin(uint32_t baud, uint8_t config);
    // overwrite Print::write
    virtual size_t write(uint8_t c);
    virtual size_t write(const uint8_t *buffer, size_t size);
    using HardwareSerial::write;
};

extern USBSerial SerialUSB;

#endif /* USBSerial_h */
