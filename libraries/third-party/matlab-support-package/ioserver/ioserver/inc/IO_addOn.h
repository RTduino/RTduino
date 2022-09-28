/**
 * @file IO_addOn.h
 *
 * Header file for IO_addOn.c
 *
 * @Copyright 2018 The MathWorks, Inc.
 *
 */
#ifndef IO_ADDON_H_
#define IO_ADDON_H_

#include "IO_utilities.h"
#ifdef __cplusplus
extern "C" {
#endif
    void addon(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
#ifdef __cplusplus
}
#endif
#endif/* IO_ADDON_H_ */
