/**
 * @file IO_utilities.h
 *
 * Header file for IO_utilities.c
 *
 * @Copyright 2018 The MathWorks, Inc.
 *
 */

#ifndef IO_UTILITIES_H_
#define IO_UTILITIES_H_

#include "IO_include.h"

#ifdef __cplusplus
extern "C" {
#endif
    uint16_T packBytesInto16BitInteger(uint8_T*);
    uint32_T packBytesInto32BitInteger(uint8_T*);
    float hexto32BitFloat(uint8_T*);
    uint16_T hexto16bitInteger(uint16_T*);
    void byteUnpack16Bits(uint8_T*, uint16_T*);
    void unpack32BitstoByte(uint32_T, uint8_T*);
#ifdef __cplusplus
}
#endif
#endif /* IO_UTILITIES_H_ */
