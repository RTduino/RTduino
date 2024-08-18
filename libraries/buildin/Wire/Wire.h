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
 * 2022-03-26     Meco Man     first version
 */
/*
    TwoWire.h - TWI/I2C library for Arduino & Wiring
    Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
    Modified December 2014 by Ivan Grokhotkov (ivan@esp8266.com) - esp8266 support
    Modified April 2015 by Hrsto Gochkov (ficeto@ficeto.com) - alternative esp8266 support
*/

#ifndef TwoWire_h
#define TwoWire_h

#include <inttypes.h>
#include <Stream.h>
#include <Arduino.h>

// DEPRECATED: Do not use BUFFER_LENGTH, prefer RTDUINO_WIRE_BUFFER_LENGTH
#ifndef RTDUINO_WIRE_BUFFER_LENGTH
#define RTDUINO_WIRE_BUFFER_LENGTH 32
#endif

#ifndef RTDUINO_DEFAULT_IIC_BUS_NAME
#define RTDUINO_DEFAULT_IIC_BUS_NAME "i2c0" /* dummy name */
#warning "Please define this macro in arduino_pinout.h and make sure this board supports I2C!"
#endif

class TwoWire : public Stream
{
private:
    bool _bus_dev_initialized;
    struct rt_i2c_bus_device *_bus_dev;

    uint8_t rxBuffer[RTDUINO_WIRE_BUFFER_LENGTH];
    size_t rxBufferIndex;
    size_t rxBufferLength;

    uint8_t txAddress;
    uint8_t txBuffer[RTDUINO_WIRE_BUFFER_LENGTH];
    size_t txBufferIndex;
    size_t txBufferLength;

    uint8_t transmitting;
    void (*user_onRequest)(void);
    void (*user_onReceive)(size_t);
    void onRequestService(void);
    void onReceiveService(uint8_t*, size_t);

    uint8_t twi_readFrom(unsigned char address, unsigned char * buf, unsigned int len, unsigned char sendStop);
    uint8_t twi_writeTo(unsigned char address, unsigned char * buf, unsigned int len, unsigned char sendStop);
    uint8_t twi_transmit(const uint8_t * buf, uint8_t len);
    uint8_t twi_status(void);

public:
    TwoWire();
    void begin(const char *i2c_dev_name = RTDUINO_DEFAULT_IIC_BUS_NAME);
    void begin(uint8_t);
    void begin(int);
    void end(void);
    void setClock(uint32_t);
    void setWireTimeout(uint32_t timeout = 25000, bool reset_with_timeout = false);
    void beginTransmission(uint8_t);
    void beginTransmission(int);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, int);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *, size_t);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    void onReceive(void (*)(int));
    void onRequest(void (*)(void));

    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }

    using Print::write;

    /* RTduino extension */
    bool isBusDeviceInited(void);
    struct rt_i2c_bus_device* getBusDevice(void);
};

extern TwoWire Wire;

#endif /* TwoWire_h */
