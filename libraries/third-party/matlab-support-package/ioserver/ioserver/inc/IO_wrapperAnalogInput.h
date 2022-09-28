/**
 * @file IO_wrapperAnalogInput.h
 *
 * Header file for IO_wrapperAnalogInput.c
 *
 * @Copyright 2017-2018 The MathWorks, Inc.
 *
 */
#ifndef IO_WRAPPERANALOGINPUT_H
#define IO_WRAPPERANALOGINPUT_H

#include "IO_peripheralInclude.h"
#ifdef ESB_BETA2_IOSERVER
#include "mw_analogin.h"
#else
#include "MW_AnalogIn.h"
#endif
#if DEBUG_FLAG
#define NUMRESULTTYPES 8
#define MAXRESULTTYPELENGTH 7
#endif

/* Create AnalogIn */
void configureAnalogInSingle(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Create AnalogIn group with Channels and Conversion time */
void configureAnalogInGroup(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Select trigger source for AnalogIn group to start conversion */
void setTriggerSourceAnalogIn(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Enable Conversion complete notification */
void enableNotificationAnalogIn(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Disable notifications */
void disableNotificationAnalogIn(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Enable continuous conversion */
void enableContConversionAnalogIn(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Set channel conversion priority */
void setChannelConvRankAnalogIn(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Get status of AnalogIn conversion group */
void getStatusAnalogIn(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Read single channel conversion result */
void readResultAnalogInSingle(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Read group conversion result */
void readResultAnalogInGroup(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Start conversion */
void startAnalogInConversion(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Stop conversion */
void stopAnalogInConversion(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Release Analog Input conversion */
void unconfigureAnalogInSingle(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* reset all Analog pins - Called on clearing the object in the host */
void resetAnalogPins(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);



#endif
