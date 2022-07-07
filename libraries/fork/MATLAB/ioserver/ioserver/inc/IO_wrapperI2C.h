/**
 * @file IO_wrapperI2C.h
 *
 * Contains declarations of functions used by the I2C IO Wrapper
 *
 * @Copyright 2017-2018 The MathWorks, Inc.
 *
 */

#ifndef IO_WRAPPERI2C_H
#define IO_WRAPPERI2C_H

#include "MW_I2C.h"
#include "IO_peripheralInclude.h"

/* Scans I2C bus and returns all 7-bit addressable slaves */
void scanI2CBus(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);
/* Takes vector of data and writes to the I2C bus */
void rawI2CWrite(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);
/* Reads data from the I2C bus */
void rawI2CRead(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);
/* Writes data to a slave's I2C configuration register */
void registerI2CWrite(uint16_T* peripheralDataSizeResponse,uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);
/* Reads data from a slave's I2C configuration register */
void registerI2CRead(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);
/* Configures I2C bus frequency */
void setI2CFrequency(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);
/* Opens the I2C bus and sets the frequency */
void openI2CBus(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);
/* Closes the I2C bus */
void closeI2CBus(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);
/* Required by Rasperry pi to mount the I2C driver module */
void enableI2C(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);
/* Required by Rasperry pi to unmount the I2C driver module */
void disableI2C(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);

#endif
