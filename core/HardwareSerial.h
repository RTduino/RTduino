/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-02-11    Meco Man     first version
 */

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include "Print.h"

// https://playground.arduino.cc/Code/Printclass/

class HardwareSerial: public Print
{
public:
    void begin(uint32_t BaudRate);
    // overwrite Print::write
    virtual size_t write(uint8_t c);
    virtual size_t write(const uint8_t *buffer, size_t size);
};

extern HardwareSerial Serial;

#endif /* HardwareSerial_h */
