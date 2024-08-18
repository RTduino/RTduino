/*
 * Copyright (c) 2021-2024, RTduino Development Team
 *
 * SPDX-License-Identifier: LGPL-v2.1
 *
 * https://github.com/RTduino/RTduino
 * https://gitee.com/rtduino/RTduino
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-04-19     Meco Man     first version
 */
/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * Copyright (c) 2014 by Matthijs Kooijman <matthijs@stdin.nl> (SPISettings AVR)
 * Copyright (c) 2014 by Andrew J. Kroll <xxxajk@gmail.com> (atomicity fixes)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <Arduino.h>

// SPI_HAS_TRANSACTION means SPI has beginTransaction(), endTransaction(),
// usingInterrupt(), and SPISetting(clock, bitOrder, dataMode)
#define SPI_HAS_TRANSACTION 1

// SPI_HAS_NOTUSINGINTERRUPT means that SPI has notUsingInterrupt() method
#define SPI_HAS_NOTUSINGINTERRUPT 0

// SPI_ATOMIC_VERSION means that SPI has atomicity fixes and what version.
// This way when there is a bug fix you can check this define to alert users
// of your code if it uses better version of this library.
// This also implies everything that SPI_HAS_TRANSACTION as documented above is
// available too.
#define SPI_ATOMIC_VERSION 1

// Uncomment this line to add detection of mismatched begin/end transactions.
// A mismatch occurs if other libraries fail to use SPI.endTransaction() for
// each SPI.beginTransaction().  Connect an LED to this pin.  The LED will turn
// on if any mismatch is ever detected.
//#define SPI_TRANSACTION_MISMATCH_LED 5

/* SPI Slave Select
   It should be defined in arduino_pinout.h
   If there is no SS definition, it will be defined as a dummy pin number.
*/

#if !defined(SS) || !defined(RTDUINO_DEFAULT_SPI_BUS_NAME)
#warning "If you don't use the RTduino SPI feature, please ignore the warnings."
#endif

#ifndef SS
#define SS      0
#warning "Please define SS pin number in arduino_pinout.h and make sure this board supports SPI!"
#endif

#ifndef RTDUINO_DEFAULT_SPI_BUS_NAME
#define RTDUINO_DEFAULT_SPI_BUS_NAME "spi0" /* dummy name */
#warning "Please define this macro in arduino_pinout.h and make sure this board supports SPI!"
#endif

#define SPI_CLOCK_DIV4 0x00
#define SPI_CLOCK_DIV16 0x01
#define SPI_CLOCK_DIV64 0x02
#define SPI_CLOCK_DIV128 0x03
#define SPI_CLOCK_DIV2 0x04
#define SPI_CLOCK_DIV8 0x05
#define SPI_CLOCK_DIV32 0x06

#define SPI_MODE0 0x00 ///<  CPOL: 0  CPHA: 0
#define SPI_MODE1 0x04 ///<  CPOL: 0  CPHA: 1
#define SPI_MODE2 0x08 ///<  CPOL: 1  CPHA: 0
#define SPI_MODE3 0x0C ///<  CPOL: 1  CPHA: 1

class SPISettings
{
    public:
        SPISettings() :_clock(1000000), _bitOrder(LSBFIRST), _dataMode(SPI_MODE0){}
        SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) :_clock(clock), _bitOrder(bitOrder), _dataMode(dataMode){}
        uint32_t _clock;
        uint8_t  _bitOrder;
        uint8_t  _dataMode;
};

class SPIClass
{
    private:
        struct rt_spi_device _chip_device;
    public:
        void begin(const char *chip_name = "RTdu",
                   const char *bus_name = RTDUINO_DEFAULT_SPI_BUS_NAME,
                   rt_base_t cs_pin = PIN_NONE);
        void beginTransaction(SPISettings settings);
        uint8_t transfer(uint8_t data);
        void transfer(void *buf, size_t count);
        uint16_t transfer16(uint16_t data);
        void endTransaction(void);
        void end(void);
        /* legacy functions */
        void setBitOrder(uint8_t bitOrder);
        void setDataMode(uint8_t dataMode);
        void setClockDivider(uint8_t clockDiv);
};

extern SPIClass SPI;

#endif
