/*
 * Copyright (c) 2021, Meco Jianting Man <jiantingman@foxmail.com>
 *
 * SPDX-License-Identifier: LGPL-v2.1
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

#include "SPI.h"

SPIClass SPI;

void SPIClass::begin(void)
{

}

void SPIClass::beginTransaction(SPISettings settings)
{

}

uint8_t SPIClass::transfer(uint8_t data)
{
    return 0;
}

void SPIClass::transfer(void *buf, size_t count)
{

}

uint16_t SPIClass::transfer16(uint16_t data)
{
    return 0;
}

void SPIClass::endTransaction(void)
{

}

void SPIClass::end(void)
{

}

void SPIClass::setBitOrder(uint8_t bitOrder)
{

}

void SPIClass::setDataMode(uint8_t dataMode)
{

}

void SPIClass::setClockDivider(uint8_t clockDiv)
{

}

void SPIClass::attachInterrupt(void)
{

}

void SPIClass::detachInterrupt(void)
{

}

void SPIClass::usingInterrupt(uint8_t interruptNumber)
{

}

void SPIClass::notUsingInterrupt(uint8_t interruptNumber)
{

}
