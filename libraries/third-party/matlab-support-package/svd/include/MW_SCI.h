/* Copyright 2016-2017 The MathWorks, Inc. */

#ifndef __MW_SCI_H__
#define __MW_SCI_H__

#include "MW_SVD.h"

typedef enum _MW_SCI_Status_Type {
    MW_SCI_SUCCESS              = 0,/*0x00*/
    MW_SCI_RX_BUSY              = 1<<0,/*0x01*/
    MW_SCI_TX_BUSY              = 1<<1,/*0x02*/
    MW_SCI_PARITY_ERROR         = 1<<2,/*0x04*/
    MW_SCI_FRAME_ERROR          = 1<<3,/*0x08*/
    MW_SCI_BUS_ERROR            = 1<<4,/*0x10*/
    MW_SCI_DATA_NOT_AVAILABLE   = 1<<5/*0x20*/
} MW_SCI_Status_Type;

typedef enum _MW_SCI_Parity_Type {
    MW_SCI_PARITY_NONE = 0,
    MW_SCI_PARITY_EVEN,
    MW_SCI_PARITY_ODD
} MW_SCI_Parity_Type;

typedef enum _MW_SCI_StopBits_Type {
    MW_SCI_STOPBITS_0_5 = 0,
    MW_SCI_STOPBITS_1,
    MW_SCI_STOPBITS_1_5,
    MW_SCI_STOPBITS_2
} MW_SCI_StopBits_Type;


typedef enum _MW_SCI_HardwareFlowControl_Type {
    MW_SCI_FLOWCONTROL_NONE = 0,
    MW_SCI_FLOWCONTROL_RTS_CTS
} MW_SCI_HardwareFlowControl_Type;

#ifdef __cplusplus
extern "C" {
#endif

/* Initialize a SCI */
MW_Handle_Type MW_SCI_Open(void * SCIModule, uint8_T isString, uint32_T RxPin, uint32_T TxPin);

/* Set SCI frame format */
MW_SCI_Status_Type MW_SCI_ConfigureHardwareFlowControl(MW_Handle_Type SCIModuleHandle, MW_SCI_HardwareFlowControl_Type HardwareFlowControl, uint32_T RtsPin, uint32_T CtsPin);

/* Set the SCI bus speed */
MW_SCI_Status_Type MW_SCI_SetBaudrate(MW_Handle_Type SCIModuleHandle, uint32_T Baudrate);

/* Set SCI frame format */
MW_SCI_Status_Type MW_SCI_SetFrameFormat(MW_Handle_Type SCIModuleHandle, uint8_T DataBits, MW_SCI_Parity_Type Parity, MW_SCI_StopBits_Type StopBits);

/* Receive the data over SCI */
MW_SCI_Status_Type MW_SCI_Receive(MW_Handle_Type SCIModuleHandle, uint8_T * RxDataPtr, uint32_T RxDataLength);

/* Trasmit the data over SCI */
MW_SCI_Status_Type MW_SCI_Transmit(MW_Handle_Type SCIModuleHandle, uint8_T * TxDataPtr, uint32_T TxDataLength);

/* Get the status of SCI device */
MW_SCI_Status_Type MW_SCI_GetStatus(MW_Handle_Type SCIModuleHandle);

/* Send break command */
MW_SCI_Status_Type MW_SCI_SendBreak(MW_Handle_Type SCIModuleHandle);

/* Release SCI module */
void MW_SCI_Close(MW_Handle_Type SCIModuleHandle);

#ifdef __cplusplus
}
#endif

#endif /* __MW_SCI_H__ */
