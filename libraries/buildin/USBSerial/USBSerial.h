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
 */

#ifndef USBSerial_h
#define USBSerial_h

#include <Stream.h>

class USBSerial: public Stream
{
public:
    void begin(void);
    void begin(uint32_t baud);
    void begin(uint32_t baud, uint8_t config);
    void end(void);
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    size_t read(uint8_t *buffer, size_t size);
    virtual int availableForWrite(void);
    virtual void flush(void);
    virtual size_t write(uint8_t c);
    virtual size_t write(const uint8_t *buffer, size_t size);
    using Print::write; // pull in write(str) from Print
};

extern USBSerial SerialUSB;

#endif /* USBSerial_h */
