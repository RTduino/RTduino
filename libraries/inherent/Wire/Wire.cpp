/*
    TwoWire.cpp - TWI/I2C library for Arduino & Wiring
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
    Modified January 2017 by Bjorn Hammarberg (bjoham@esp8266.com) - i2c slave support
*/

extern "C" {
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
}
#include <rtdevice.h>
#include "Wire.h"

#define DBG_TAG    "Arduino.library.Wire"
#define DBG_LVL    DBG_INFO
#include <rtdbg.h>

// Private /////////////////////////////////////////////////////////////////////

/*
 * Function twi_readFrom
 * Desc     attempts to become twi bus master and read a
 *          series of bytes from a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes to read into array
 * Output   number of bytes read
 */
uint8_t TwoWire::twi_readFrom(unsigned char address, unsigned char * buf, unsigned int len, unsigned char sendStop)
{
    rt_uint16_t flag = RT_NULL;

    if(sendStop == false)
    {
        flag |= RT_I2C_NO_STOP;
    }

    return rt_i2c_master_recv(_i2c_bus_dev, address, flag, buf, len);
}

uint8_t TwoWire::twi_writeTo(unsigned char address, unsigned char * buf, unsigned int len, unsigned char sendStop)
{
    rt_uint16_t flag = RT_NULL;

    if(sendStop == false)
    {
        flag |= RT_I2C_NO_STOP;
    }

    return rt_i2c_master_send(_i2c_bus_dev, address, flag, buf, len);
}

/* 
 * Function twi_transmit
 * Desc     fills slave tx buffer with data
 *          must be called in slave tx event callback
 * Input    data: pointer to byte array
 *          length: number of bytes in array
 * Output   1 length too long for buffer
 *          2 not slave transmitter
 *          0 ok
 */
uint8_t TwoWire::twi_transmit(const uint8_t * buf, uint8_t len)
{
    rt_i2c_master_send(_i2c_bus_dev, txAddress, RT_I2C_NO_START | RT_I2C_NO_STOP, buf, len);
    return 0;
}

uint8_t TwoWire::twi_status(void)
{
    return 0;
}

// Constructors ////////////////////////////////////////////////////////////////

TwoWire::TwoWire()
{
    _i2c_bus_dev = RT_NULL;
    rt_memset(rxBuffer, 0, I2C_BUFFER_LENGTH);
    rt_memset(txBuffer, 0, I2C_BUFFER_LENGTH);
    rxBufferIndex = 0;
    rxBufferLength = 0;
    txAddress = 0;
    txBufferIndex = 0;
    txBufferLength = 0;
    transmitting = 0;
    user_onRequest = NULL;
    user_onReceive = NULL;
}

// Public Methods //////////////////////////////////////////////////////////////

void TwoWire::begin(const char *i2c_dev_name)
{
    struct rt_i2c_bus_device * dev;

    dev = (struct rt_i2c_bus_device *)rt_device_find(i2c_dev_name);
    if(dev == RT_NULL)
    {
        LOG_E("Cannot find IIC device!");
        return;
    }
    _i2c_bus_dev = dev;
}

void TwoWire::begin(uint8_t address)
{
}

void TwoWire::begin(int address)
{
    begin((uint8_t)address);
}

uint8_t TwoWire::status()
{
    return twi_status();
}

void TwoWire::setClock(uint32_t frequency)
{

}

void TwoWire::setClockStretchLimit(uint32_t limit)
{

}

size_t TwoWire::requestFrom(uint8_t address, size_t size, bool sendStop)
{
    if (size > I2C_BUFFER_LENGTH)
    {
        size = I2C_BUFFER_LENGTH;
    }
    size_t read = (twi_readFrom(address, rxBuffer, size, sendStop) == 0) ? size : 0;
    rxBufferIndex = 0;
    rxBufferLength = read;
    return read;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
    return requestFrom(address, static_cast<size_t>(quantity), static_cast<bool>(sendStop));
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
    return requestFrom(address, static_cast<size_t>(quantity), true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
    return requestFrom(static_cast<uint8_t>(address), static_cast<size_t>(quantity), true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
    return requestFrom(static_cast<uint8_t>(address), static_cast<size_t>(quantity), static_cast<bool>(sendStop));
}

void TwoWire::beginTransmission(uint8_t address)
{
    transmitting = 1;
    txAddress = address;
    txBufferIndex = 0;
    txBufferLength = 0;
}

void TwoWire::beginTransmission(int address)
{
    beginTransmission((uint8_t)address);
}

uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
    int8_t ret = twi_writeTo(txAddress, txBuffer, txBufferLength, sendStop);
    txBufferIndex = 0;
    txBufferLength = 0;
    transmitting = 0;
    return ret;
}

uint8_t TwoWire::endTransmission(void)
{
    return endTransmission(true);
}

size_t TwoWire::write(uint8_t data)
{
    if (transmitting)
    {
        if (txBufferLength >= I2C_BUFFER_LENGTH)
        {
            setWriteError();
            return 0;
        }
        txBuffer[txBufferIndex] = data;
        ++txBufferIndex;
        txBufferLength = txBufferIndex;
    }
    else
    {
        twi_transmit(&data, 1);
    }
    return 1;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
    if (transmitting)
    {
        for (size_t i = 0; i < quantity; ++i)
        {
            if (!write(data[i]))
            {
                return i;
            }
        }
    }
    else
    {
        twi_transmit(data, quantity);
    }
    return quantity;
}

int TwoWire::available(void)
{
    return rxBufferLength - rxBufferIndex;
}

int TwoWire::read(void)
{
    int value = -1;
    if (rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex];
        ++rxBufferIndex;
    }
    return value;
}

int TwoWire::peek(void)
{
    int value = -1;
    if (rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex];
    }
    return value;
}

void TwoWire::flush(void)
{
    rxBufferIndex = 0;
    rxBufferLength = 0;
    txBufferIndex = 0;
    txBufferLength = 0;
}

void TwoWire::onReceiveService(uint8_t* inBytes, size_t numBytes)
{
    // don't bother if user hasn't registered a callback
    if (!user_onReceive)
    {
        return;
    }
    // // don't bother if rx buffer is in use by a master requestFrom() op
    // // i know this drops data, but it allows for slight stupidity
    // // meaning, they may not have read all the master requestFrom() data yet
    // if(rxBufferIndex < rxBufferLength){
    //   return;
    // }

    // copy twi rx buffer into local read buffer
    // this enables new reads to happen in parallel
    for (uint8_t i = 0; i < numBytes; ++i)
    {
        rxBuffer[i] = inBytes[i];
    }

    // set rx iterator vars
    rxBufferIndex = 0;
    rxBufferLength = numBytes;

    // alert user program
    user_onReceive(numBytes);
}

void TwoWire::onRequestService(void)
{
    // don't bother if user hasn't registered a callback
    if (!user_onRequest)
    {
        return;
    }

    // reset tx buffer iterator vars
    // !!! this will kill any pending pre-master sendTo() activity
    txBufferIndex = 0;
    txBufferLength = 0;

    // alert user program
    user_onRequest();
}

//void TwoWire::onReceive(void (*function)(int))
//{
//    // arduino api compatibility fixer:
//    // really hope size parameter will not exceed 2^31 :)
//    static_assert(sizeof(int) == sizeof(size_t), "something is wrong in Arduino kingdom");
//    user_onReceive = reinterpret_cast<void(*)(size_t)>(function);
//}

//void TwoWire::onReceive(void (*function)(size_t))
//{
//    user_onReceive = function;
//    twi_enableSlaveMode();
//}

//void TwoWire::onRequest(void (*function)(void))
//{
//    user_onRequest = function;
//    twi_enableSlaveMode();
//}

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire;
