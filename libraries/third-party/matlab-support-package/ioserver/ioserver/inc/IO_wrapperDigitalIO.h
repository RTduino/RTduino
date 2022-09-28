/**
 * @file IO_wrapperDigitalIO.h
 *
 * Header file for IO_wrapperDigitalIO.c
 *
 * @Copyright 2017-2018 The MathWorks, Inc.
 *
 */
#ifndef IO_WRAPPERDIGITALIO_H
#define IO_WRAPPERDIGITALIO_H

#include "IO_peripheralInclude.h"
#ifdef ESB_BETA2_IOSERVER
#include "mw_digitalio.h"
#else
#include "MW_digitalIO.h"
#endif

/* Configures a digital pin as input or output */
void configureDigitalPin(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Sets or clears a digital pin */
void writeDigitalPin(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Reads the status of a digital pin */
void readDigitalPin(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Unconfigures a digital pin */
void unconfigureDigitalPin(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* reset all Digital pins - Called on clearing the object in the host */
void resetDigitalPins(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);

#endif
