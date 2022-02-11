/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-02-11    Meco Man     first version
 */

#ifndef ConsoleSerial_h
#define ConsoleSerial_h

#include "Print.h"

// https://playground.arduino.cc/Code/Printclass/

class ConsoleSerial: public Print
{
public:
    ConsoleSerial(void);
    void begin(uint32_t BaudRate);
    // overwrite Print::write
    virtual size_t write(uint8_t c);
    virtual size_t write(const uint8_t *buffer, size_t size);
};

extern ConsoleSerial Serial;

#endif /* ConsoleSerial_h */
