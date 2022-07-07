/**
 * @file IO_wrapperSCI.h
 *
 * Header file for IO_wrapperSCI.c
 *
 * @Copyright 2019 The MathWorks, Inc.
 *
 */
#ifndef IO_WRAPPERSCI_H
#define IO_WRAPPERSCI_H

#include "IO_peripheralInclude.h"
#include "MW_SCI.h"

/* Initialize a SCI */
void openSCIBus(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Set SCI frame format */
void configureSCIHardwareFlowControl(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
#ifdef LTC_BAREMETAL_HARDWARE
MW_SCI_Status_Type MW_SCI_ConfigureTimeOut(MW_Handle_Type SCIModuleHandle, uint32_T timeOut);
/* Set SCI timeout*/
void setSCITimeOut(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
#endif
/* Set the SCI bus speed */
void setSCIBaudrate(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Set SCI frame format */
void setSCIFrameFormat(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Receive the data over SCI */
void sciReceive(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Trasmit the data over SCI */
void sciTrasmit(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Get the status of SCI device */
void getSCIStatus(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Send break command */
void sciSendBreak(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Release SCI module */
void sciClose(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);

#endif
