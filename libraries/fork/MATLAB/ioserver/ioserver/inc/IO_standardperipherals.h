/**
 * @file IO_standardperipherals.h
 *
 * Contains declarations of functions used by the standard peripherals
 *
 * @Copyright 2017-2018 The MathWorks, Inc.
 *
 */

#ifndef IO_STANDARDPERIPHERALS_H
#define IO_STANDARDPERIPHERALS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "IO_peripheralInclude.h"

uint8_T standardPeripherals(uint16_T ,uint8_T* , uint8_T* ,uint16_T* ,uint8_T*);
    void standardPeripheralsInit(void);

#ifdef __cplusplus
}
#endif

#endif //IO_STANDARDPERIPHERALS_H
