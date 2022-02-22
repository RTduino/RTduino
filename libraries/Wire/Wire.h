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
#include <rtdevice.h>
#include <Stream.h>
#include <Arduino.h>

#ifndef I2C_BUFFER_LENGTH
// DEPRECATED: Do not use BUFFER_LENGTH, prefer I2C_BUFFER_LENGTH
#define BUFFER_LENGTH 128
#define I2C_BUFFER_LENGTH BUFFER_LENGTH
#endif

class TwoWire : public Stream
{
private:
    uint8_t rxBuffer[I2C_BUFFER_LENGTH];
    size_t rxBufferIndex;
    size_t rxBufferLength;

    uint8_t txAddress;
    uint8_t txBuffer[I2C_BUFFER_LENGTH];
    size_t txBufferIndex;
    size_t txBufferLength;

    uint8_t transmitting;
    void (*user_onRequest)(void);
    void (*user_onReceive)(size_t);
    void onRequestService(void);
    void onReceiveService(uint8_t*, size_t);
public:
    struct rt_i2c_bus_device *_i2c_bus_dev;
    TwoWire();
    void begin(const char *i2c_dev_name = ARDUINO_DEFAULT_IIC_BUS_NAME);
    void begin(uint8_t);
    void begin(int);
    void setClock(uint32_t);
    void setClockStretchLimit(uint32_t);
    void beginTransmission(uint8_t);
    void beginTransmission(int);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t);
    size_t requestFrom(uint8_t address, size_t size, bool sendStop);
    uint8_t status();

    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, int);

    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *, size_t);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    void onReceive(void (*)(int));      // arduino api
    void onReceive(void (*)(size_t));   // legacy esp8266 backward compatibility
    void onRequest(void (*)(void));

    using Print::write;
};

extern TwoWire Wire;

#endif /* TwoWire_h */
