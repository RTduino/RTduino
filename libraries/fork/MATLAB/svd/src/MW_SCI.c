/* Copyright 2016 The MathWorks, Inc. */

#include "MW_SCI.h"


/* Initialize a SCI */
MW_Handle_Type MW_SCI_Open(void * SCIModule, uint8_T isString, uint32_T RxPin, uint32_T TxPin)
{
    return 0;
}

/* Set SCI frame format */
MW_SCI_Status_Type MW_SCI_ConfigureHardwareFlowControl(MW_Handle_Type SCIModuleHandle, MW_SCI_HardwareFlowControl_Type HardwareFlowControl, uint32_T RtsDtrPin, uint32_T CtsDtsPin)
{
    return MW_SCI_SUCCESS;
}

/* Set the SCI bus speed */
MW_SCI_Status_Type MW_SCI_SetBaudrate(MW_Handle_Type SCIModuleHandle, uint32_T Baudrate)
{
    return MW_SCI_SUCCESS;
}

/* Set SCI frame format */
MW_SCI_Status_Type MW_SCI_SetFrameFormat(MW_Handle_Type SCIModuleHandle, uint8_T DataBitsLength, MW_SCI_Parity_Type Parity, MW_SCI_StopBits_Type StopBits)
{
    return MW_SCI_SUCCESS;
}

/* Receive the data over SCI */
MW_SCI_Status_Type MW_SCI_Receive(MW_Handle_Type SCIModuleHandle, uint8_T * RxDataPtr, uint32_T RxDataLength)
{
    return MW_SCI_SUCCESS;
}

/* Trasmit the data over SCI */
MW_SCI_Status_Type MW_SCI_Transmit(MW_Handle_Type SCIModuleHandle, uint8_T * TxDataPtr, uint32_T TxDataLength)
{
    return MW_SCI_SUCCESS;
}

/* Get the status of SCI device */
MW_SCI_Status_Type MW_SCI_GetStatus(MW_Handle_Type SCIModuleHandle)
{
    return MW_SCI_SUCCESS;
}

/* Send break command */
MW_SCI_Status_Type MW_SCI_SendBreak(MW_Handle_Type SCIModuleHandle)
{
    return MW_SCI_SUCCESS;
}

/* Release SCI module */
void MW_SCI_Close(MW_Handle_Type SCIModuleHandle)
{

}
