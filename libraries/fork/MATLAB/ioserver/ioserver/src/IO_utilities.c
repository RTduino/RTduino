/**
 * @file IO_utilities.c
 *
 * Defines functions used across IO Server
 *
 * @Copyright 2018 The MathWorks, Inc.
 *
 */

#include "IO_utilities.h"

uint16_T packBytesInto16BitInteger(uint8_T* data)
{
    return (uint16_T)(((uint16_T)data[0]) | (((uint16_T)data[1])<<8));
}

uint32_T packBytesInto32BitInteger(uint8_T* data)
{
    return (uint32_T)(((uint32_T)data[0]) | (((uint32_T)data[1])<<8) | (((uint32_T)data[2])<<16) | (((uint32_T)data[3])<<24));
}

float hexto32BitFloat(uint8_T* data)
{
    float floatTemp = 0;
    uint32_T intTemp = 0;
    intTemp = (uint32_T)((uint32_T)data[0] | ((uint32_T)data[1]<<8) | ((uint32_T)data[2]<<16) | ((uint32_T)data[3]<<24));
    memcpy(&floatTemp,&intTemp,sizeof(float));
    return floatTemp;
}

void byteUnpack16Bits(uint8_T* dst, uint16_T* src)
{
    uint8_T index = 0;
    for( index = 0; index < 2; index++)
    {
        dst[index] = (uint8_T)(((uint16_T)*src >> (8 * index)) & (uint16_T)0x00ff);
    }
}

void unpack32BitstoByte(uint32_T value, uint8_T *data)
{
    uint16_T datauint16 = value & 0xFFFF;
    byteUnpack16Bits(&data[0],&datauint16);
    datauint16 = (value>>16) & 0xFFFF;
    byteUnpack16Bits(&data[2],&datauint16);
}
