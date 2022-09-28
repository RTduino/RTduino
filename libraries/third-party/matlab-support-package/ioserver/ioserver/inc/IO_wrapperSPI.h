/**
 * @file IO_wrapperSPI.h
 *
 * Contains declarations of functions used by the SPI IO Wrapper
 *
 * @Copyright 2017-2018 The MathWorks, Inc.
 *
 */

#ifndef IO_WRAPPERSPI_H
#define IO_WRAPPERSPI_H


#include "MW_SPI.h"
#include "MW_digitalIO.h"
#include "IO_peripheralInclude.h"

/* enableSPI function to enable the SPI device peripheral */
void enableSPI(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);
/* disableSPI function to disable the SPI device peripheral */
void disableSPI(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);
/* openSPI function to setup and initialize the SPI device properties and to open the SPI bus */
void openSPI(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);
/* setFormatSPI is used to set the SPI Mode and SPI bit order*/
void setFormatSPI(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);
/* setBusSpeedSPI is used to set the SPI bus frequency*/
void setBusSpeedSPI(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);
/* writeReadSPI function to write and read data to or from an SPI device */
void writeReadSPI(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);
/* closeSPI function to close the SPI bus */
void closeSPI(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status);

#endif //IO_WRAPPERSPI_H
