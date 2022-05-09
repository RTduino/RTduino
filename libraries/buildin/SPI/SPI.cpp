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

#define DBG_TAG    "Arduino.SPI"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

SPIClass SPI;

void SPIClass::begin(const char *spi_bus_name)
{
    rt_spi_bus_attach_device(&this->spi_device, "spiardev", spi_bus_name, NULL);
}

void SPIClass::beginTransaction(SPISettings settings)
{
    struct rt_spi_configuration cfg;

    RT_ASSERT(settings._bitOrder == LSBFIRST || settings._bitOrder == MSBFIRST);
    RT_ASSERT(settings._dataMode == SPI_MODE0 ||
              settings._dataMode == SPI_MODE1 ||
              settings._dataMode == SPI_MODE2 ||
              settings._dataMode == SPI_MODE3);

    cfg.mode = RT_SPI_MASTER | RT_SPI_NO_CS;
    cfg.data_width = 8;
    cfg.max_hz = settings._clock;

    if(settings._bitOrder == LSBFIRST)
    {
        cfg.mode |= RT_SPI_LSB;
    }
    else /*MSBFIRST*/
    {
        cfg.mode |= RT_SPI_MSB;
    }

    if(settings._dataMode == SPI_MODE0)
    {
        cfg.mode |= RT_SPI_MODE_0;
    }
    else if(settings._dataMode == SPI_MODE1)
    {
        cfg.mode |= RT_SPI_MODE_1;
    }
    else if(settings._dataMode == SPI_MODE2)
    {
        cfg.mode |= RT_SPI_MODE_2;
    }
    else /*SPI_MODE3*/
    {
        cfg.mode |= RT_SPI_MODE_3;
    }

    rt_spi_configure(&this->spi_device, &cfg);

    rt_spi_take_bus(&this->spi_device);
}

uint8_t SPIClass::transfer(uint8_t data)
{
    return rt_spi_sendrecv8(&this->spi_device, data);
}

void SPIClass::transfer(void *buf, size_t count)
{
    rt_err_t err;
    void *recv_buf;

    recv_buf = rt_malloc(count);
    if(recv_buf == RT_NULL)
    {
        return;
    }

    err = rt_spi_transfer(&this->spi_device, buf, recv_buf, count);
    if(err != RT_EOK)
    {
        rt_free(recv_buf);
        return;
    }

    rt_memcpy(buf, recv_buf, count);
    rt_free(recv_buf);
}

uint16_t SPIClass::transfer16(uint16_t data)
{
    return rt_spi_sendrecv16(&this->spi_device, data);
}

void SPIClass::endTransaction(void)
{
    rt_spi_release_bus(&this->spi_device);
}

void SPIClass::end(void)
{
    /* Nothing to do. RTduino has no right to shutdown RT-Thread SPI devcies */
}

/* legacy functions */
void SPIClass::setBitOrder(uint8_t bitOrder)
{
}

void SPIClass::setDataMode(uint8_t dataMode)
{
}

void SPIClass::setClockDivider(uint8_t clockDiv)
{
}
