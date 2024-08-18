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
 * 2022-03-26     Meco Man     first version
 */
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

#define DBG_TAG    "RTduino.Wire"
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

    if (!sendStop)
    {
        flag |= RT_I2C_NO_STOP;
    }

    return rt_i2c_master_recv(_bus_dev, address, flag, buf, len);
}

/*
 * Function twi_writeTo
 * Desc     attempts to become twi bus master and write a
 *          series of bytes to a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes in array
 *          wait: boolean indicating to wait for write or not
 * Output   0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 */
uint8_t TwoWire::twi_writeTo(unsigned char address, unsigned char * buf, unsigned int len, unsigned char sendStop)
{
    rt_uint16_t flag = RT_NULL;

    if (!sendStop)
    {
        flag |= RT_I2C_NO_STOP;
    }

    if(rt_i2c_master_send(_bus_dev, address, flag, buf, len) != 0)
    {
        return 0; //success
    }
    else
    {
        return 4;
    }
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
    int8_t ret = rt_i2c_master_send(_bus_dev, txAddress, RT_NULL, buf, len);

    if(ret > 0)
    {
        return 0; // success
    }
    else
    {
        return 4; // other error
    }
}

/*
 * Function twi_disable
 * Desc     disables twi pins
 * Input    none
 * Output   none
 */
void twi_disable(void)
{
}

// Constructors ////////////////////////////////////////////////////////////////

TwoWire::TwoWire()
{
    _bus_dev = RT_NULL;
    _bus_dev_initialized = false;
    rt_memset(rxBuffer, 0, RTDUINO_WIRE_BUFFER_LENGTH);
    rt_memset(txBuffer, 0, RTDUINO_WIRE_BUFFER_LENGTH);
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
    if (_bus_dev_initialized)
    {
        return;
    }

    struct rt_i2c_bus_device *dev;

    dev = (struct rt_i2c_bus_device *)rt_device_find(i2c_dev_name);
    if(dev == RT_NULL)
    {
        LOG_E("Cannot find device: %s!", i2c_dev_name);
        return;
    }

    rxBufferIndex = 0;
    rxBufferLength = 0;

    txBufferIndex = 0;
    txBufferLength = 0;

    _bus_dev = dev;
    _bus_dev_initialized = true;
}

//void TwoWire::begin(uint8_t address)
//{
//}
//
//void TwoWire::begin(int address)
//{
//    begin((uint8_t)address);
//}

void TwoWire::end(void)
{
    twi_disable();
}

void TwoWire::setClock(uint32_t frequency)
{
    /* Nothing needs to do */
    RT_UNUSED(frequency);
}

/***
 * Sets the TWI timeout.
 *
 * This limits the maximum time to wait for the TWI hardware. If more time passes, the bus is assumed
 * to have locked up (e.g. due to noise-induced glitches or faulty slaves) and the transaction is aborted.
 * Optionally, the TWI hardware is also reset, which can be required to allow subsequent transactions to
 * succeed in some cases (in particular when noise has made the TWI hardware think there is a second
 * master that has claimed the bus).
 *
 * When a timeout is triggered, a flag is set that can be queried with `getWireTimeoutFlag()` and is cleared
 * when `clearWireTimeoutFlag()` or `setWireTimeoutUs()` is called.
 *
 * Note that this timeout can also trigger while waiting for clock stretching or waiting for a second master
 * to complete its transaction. So make sure to adapt the timeout to accommodate for those cases if needed.
 * A typical timeout would be 25ms (which is the maximum clock stretching allowed by the SMBus protocol),
 * but (much) shorter values will usually also work.
 *
 * In the future, a timeout will be enabled by default, so if you require the timeout to be disabled, it is
 * recommended you disable it by default using `setWireTimeoutUs(0)`, even though that is currently
 * the default.
 *
 * @param timeout a timeout value in microseconds, if zero then timeout checking is disabled
 * @param reset_with_timeout if true then TWI interface will be automatically reset on timeout
 *                           if false then TWI interface will not be reset on timeout
 */
void TwoWire::setWireTimeout(uint32_t timeout, bool reset_with_timeout)
{
    /* Nothing needs to do */
    RT_UNUSED(timeout);
    RT_UNUSED(reset_with_timeout);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
  if (isize > 0) {
  // send internal address; this mode allows sending a repeated start to access
  // some devices' internal registers. This function is executed by the hardware
  // TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)

  beginTransmission(address);

  // the maximum size of internal address is 3 bytes
  if (isize > 3){
    isize = 3;
  }

  // write internal register address - most significant byte first
  while (isize-- > 0)
    write((uint8_t)(iaddress >> (isize*8)));
  endTransmission(false);
  }

  // clamp to buffer length
  if(quantity > RTDUINO_WIRE_BUFFER_LENGTH){
    quantity = RTDUINO_WIRE_BUFFER_LENGTH;
  }
  // perform blocking read into buffer
  uint8_t read = twi_readFrom(address, rxBuffer, quantity, sendStop);
  // set rx buffer iterator vars
  rxBufferIndex = 0;
  rxBufferLength = read;

  return read;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
    return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}


void TwoWire::beginTransmission(uint8_t address)
{
    // indicate that we are transmitting
    transmitting = 1;
    // set address of targeted slave
    txAddress = address;
    // reset tx buffer iterator vars
    txBufferIndex = 0;
    txBufferLength = 0;
}

void TwoWire::beginTransmission(int address)
{
    beginTransmission((uint8_t)address);
}

//
//  Originally, 'endTransmission' was an f(void) function.
//  It has been modified to take one parameter indicating
//  whether or not a STOP should be performed on the bus.
//  Calling endTransmission(false) allows a sketch to
//  perform a repeated start.
//
//  WARNING: Nothing in the library keeps track of whether
//  the bus tenure has been properly ended with a STOP. It
//  is very possible to leave the bus in a hung state if
//  no call to endTransmission(true) is made. Some I2C
//  devices will behave oddly if they do not see a STOP.
//
//Returns
//byte, which indicates the status of the transmission:
//0:success
//1:data too long to fit in transmit buffer
//2:received NACK on transmit of address
//3:received NACK on transmit of data
//4:other error
uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
    // transmit buffer (blocking)
    int8_t ret =twi_writeTo(txAddress, txBuffer, txBufferLength, sendStop);

    // reset tx buffer iterator vars
    txBufferIndex = 0;
    txBufferLength = 0;

    // indicate that we are done transmitting
    transmitting = 0;

    return ret;

}

uint8_t TwoWire::endTransmission(void)
{
    return endTransmission(true);
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(uint8_t data)
{
    if (transmitting)
    {
        if (txBufferLength >= RTDUINO_WIRE_BUFFER_LENGTH)
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
        // in slave send mode
        // reply to master
        twi_transmit(&data, 1);
    }
    return 1;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
    if (transmitting)
    {
        // in master transmitter mode
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
        // in slave send mode
        // reply to master
        twi_transmit(data, quantity);
    }
    return quantity;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::available(void)
{
    return rxBufferLength - rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::read(void)
{
    int value = -1;

    // get each successive byte on each call
    if (rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex];
        ++rxBufferIndex;
    }
    return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
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
    // Arduino original code says that this part needs to be implemented.
    rxBufferIndex = 0;
    rxBufferLength = 0;
    txBufferIndex = 0;
    txBufferLength = 0;
}

//// behind the scenes function that is called when data is received
//void TwoWire::onReceiveService(uint8_t* inBytes, int numBytes)
//{
//    // don't bother if user hasn't registered a callback
//    if(!user_onReceive){
//        return;
//    }
//    // don't bother if rx buffer is in use by a master requestFrom() op
//    // i know this drops data, but it allows for slight stupidity
//    // meaning, they may not have read all the master requestFrom() data yet
//    if(rxBufferIndex < rxBufferLength){
//        return;
//    }
//    // copy twi rx buffer into local read buffer
//    // this enables new reads to happen in parallel
//    for(uint8_t i = 0; i < numBytes; ++i){
//        rxBuffer[i] = inBytes[i];
//    }
//    // set rx iterator vars
//    rxBufferIndex = 0;
//    rxBufferLength = numBytes;
//    // alert user program
//    user_onReceive(numBytes);
//}

//// behind the scenes function that is called when data is requested
//void TwoWire::onRequestService(void)
//{
//    // don't bother if user hasn't registered a callback
//    if(!user_onRequest){
//        return;
//    }
//    // reset tx buffer iterator vars
//    // !!! this will kill any pending pre-master sendTo() activity
//    txBufferIndex = 0;
//    txBufferLength = 0;
//    // alert user program
//    user_onRequest();
//}

//// sets function called on slave write
//void TwoWire::onReceive( void (*function)(int) )
//{
//    user_onReceive = function;
//}
//

//// sets function called on slave read
//void TwoWire::onRequest( void (*function)(void) )
//{
//    user_onRequest = function;
//}

bool TwoWire::isBusDeviceInited(void)
{
    return _bus_dev_initialized;
}

struct rt_i2c_bus_device* TwoWire::getBusDevice(void)
{
    return _bus_dev;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire;
