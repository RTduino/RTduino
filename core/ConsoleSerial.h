/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-03-24     Meco Man     first version
 */

#ifndef ConsoleSerial_h
#define ConsoleSerial_h

#include <HardwareSerial.h>

class ConsoleSerial: public HardwareSerial
{
public:
    void begin(uint32_t baud);
    void begin(uint32_t baud, uint8_t config);
    // overwrite Print::write
    virtual size_t write(uint8_t c);
    virtual size_t write(const uint8_t *buffer, size_t size);
    using HardwareSerial::write;
};

extern ConsoleSerial Serial;

#endif /* ConsoleSerial_h */
