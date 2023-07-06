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

#define DBG_TAG    "RTduino.SPI"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

SPIClass SPI;

void SPIClass::begin(const char *spi_bus_name)
{
    if(rt_spi_bus_attach_device(&this->spi_device, "RTduSPI", spi_bus_name, RT_NULL) != RT_EOK)
    {
        LOG_E("SPI device fail to attach!");
    }
}

void SPIClass::beginTransaction(SPISettings settings)
{
    struct rt_spi_configuration cfg = {0};

    RT_ASSERT(settings._bitOrder == LSBFIRST || settings._bitOrder == MSBFIRST);
    RT_ASSERT(settings._dataMode == SPI_MODE0 ||
              settings._dataMode == SPI_MODE1 ||
              settings._dataMode == SPI_MODE2 ||
              settings._dataMode == SPI_MODE3);

    /* Don't need to RT-Thread SPI device driver to control CS */
    cfg.mode = RT_SPI_MASTER | RT_SPI_NO_CS;
    cfg.data_width = 8;
    cfg.max_hz = settings._clock;

    if(settings._bitOrder == LSBFIRST)
    {
        cfg.mode |= RT_SPI_LSB;
    }
    else if (settings._bitOrder == MSBFIRST)
    {
        cfg.mode |= RT_SPI_MSB;
    }
    else
    {
        return;
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
    else if(settings._dataMode == SPI_MODE3)
    {
        cfg.mode |= RT_SPI_MODE_3;
    }
    else
    {
        return;
    }

    rt_spi_configure(&this->spi_device, &cfg);
    rt_spi_take_bus(&this->spi_device);
}

uint8_t SPIClass::transfer(uint8_t data)
{
#if RT_VER_NUM >= 0x50000
    uint8_t recvdata;
    if(rt_spi_sendrecv8(&this->spi_device, data, &recvdata) != RT_EOK)
    {
        recvdata = 0x00U;
    }
    return recvdata;
#else
    return rt_spi_sendrecv8(&this->spi_device, data);
#endif
}

void SPIClass::transfer(void *buf, size_t count)
{
    rt_spi_transfer(&this->spi_device, buf, buf, count);
}

uint16_t SPIClass::transfer16(uint16_t data)
{
#if RT_VER_NUM >= 0x50000
    rt_uint16_t recvdata;
    if(rt_spi_sendrecv16(&this->spi_device, data, &recvdata) != RT_EOK)
    {
        recvdata = 0x00U;
    }
    return recvdata;
#else
    return rt_spi_sendrecv16(&this->spi_device, data);
#endif /* RT_VER_NUM >= 0x50000 */
}

void SPIClass::endTransaction(void)
{
    rt_spi_release_bus(&this->spi_device);
}

void SPIClass::end(void)
{
    /* Nothing to do. RTduino has no right to shutdown RT-Thread SPI devcies */
}

/* ----------------------- legacy functions -------------------------- */
void SPIClass::setBitOrder(uint8_t bitOrder)
{
    rt_uint8_t temp_mode = this->spi_device.config.mode;

    RT_ASSERT(bitOrder == LSBFIRST || bitOrder == MSBFIRST);

    if(bitOrder == LSBFIRST)
    {
        temp_mode &= ~RT_SPI_MSB; /* clear bit */
    }
    else if(bitOrder == MSBFIRST)
    {
        temp_mode |= RT_SPI_MSB; /* set bit */
    }
    else
    {
        return;
    }

    this->spi_device.config.mode = temp_mode;
    this->spi_device.bus->ops->configure(&this->spi_device, &this->spi_device.config);
}

void SPIClass::setDataMode(uint8_t dataMode)
{
    rt_uint8_t temp_mode = this->spi_device.config.mode;

    RT_ASSERT(dataMode == SPI_MODE0 ||
              dataMode == SPI_MODE1 ||
              dataMode == SPI_MODE2 ||
              dataMode == SPI_MODE3);

    temp_mode &= ~RT_SPI_MODE_MASK;

    if(dataMode == SPI_MODE0)
    {
        temp_mode |= RT_SPI_MODE_0;
    }
    else if(dataMode == SPI_MODE1)
    {
        temp_mode |= RT_SPI_MODE_1;
    }
    else if(dataMode == SPI_MODE2)
    {
        temp_mode |= RT_SPI_MODE_2;
    }
    else if(dataMode == SPI_MODE3)
    {
        temp_mode |= RT_SPI_MODE_3;
    }
    else
    {
        return;
    }

    this->spi_device.config.mode = temp_mode;
    this->spi_device.bus->ops->configure(&this->spi_device, &this->spi_device.config);
}

void SPIClass::setClockDivider(uint8_t clockDiv)
{
    this->spi_device.config.max_hz = 100000;
    this->spi_device.bus->ops->configure(&this->spi_device, &this->spi_device.config);
}
