/*
 * Copyright (c) 2021-2022, RTduino Development Team
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * https://github.com/RTduino/RTduino
 * https://gitee.com/rtduino/RTduino
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-02-11     Meco Man     first version
 */

#ifndef HardwareSerial_h
#define HardwareSerial_h

#include <rtthread.h>
#include "Stream.h"

#ifndef RTDUINO_TINY_MODE
#include <pins_arduino.h>
#endif

// Define config for Serial.begin(baud, config);
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06
#define SERIAL_5N2 0x08
#define SERIAL_6N2 0x0A
#define SERIAL_7N2 0x0C
#define SERIAL_8N2 0x0E
#define SERIAL_5E1 0x20
#define SERIAL_6E1 0x22
#define SERIAL_7E1 0x24
#define SERIAL_8E1 0x26
#define SERIAL_5E2 0x28
#define SERIAL_6E2 0x2A
#define SERIAL_7E2 0x2C
#define SERIAL_8E2 0x2E
#define SERIAL_5O1 0x30
#define SERIAL_6O1 0x32
#define SERIAL_7O1 0x34
#define SERIAL_8O1 0x36
#define SERIAL_5O2 0x38
#define SERIAL_6O2 0x3A
#define SERIAL_7O2 0x3C
#define SERIAL_8O2 0x3E

class HardwareSerial: public Stream
{
protected:
    rt_device_t uart_dev;

public:
    HardwareSerial(rt_device_t device);
    HardwareSerial(const char* dev_name);
    virtual ~HardwareSerial() {}
    void begin(void);
    void begin(uint32_t baud);
    void begin(uint32_t baud, uint8_t config);
    void end(void);
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual int availableForWrite(void);
    virtual void flush(void);
    virtual size_t write(uint8_t c);
    virtual size_t write(const uint8_t *buffer, size_t size);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    using Print::write; // pull in write(str) and write(buf, size) from Print
    operator bool() { return true; }
};

class ConsoleSerial: public HardwareSerial
{
public:
    ConsoleSerial():HardwareSerial(rt_console_get_device()){};
#ifdef RTDUINO_CMD_SERIAL_USING_FOCUSON
    int available(void);
    int peek(void);
    int read(void);
#endif /* RTDUINO_CMD_SERIAL_USING_FOCUSON */
};

extern ConsoleSerial Serial;
#ifdef RTDUINO_SERIAL2_DEVICE_NAME
extern HardwareSerial Serial2;
#endif
#ifdef RTDUINO_SERIAL3_DEVICE_NAME
extern HardwareSerial Serial3;
#endif

#endif /* HardwareSerial_h */
