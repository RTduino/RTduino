/* Copyright 2017 The MathWorks, Inc. */
#ifndef CUSTOMFUNCTION_H
#define CUSTOMFUNCTION_H

//Uncomment the line below to enable debug option
//#define DEBUG_FLAG 1

#include "IO_include.h"
#include "IO_peripheralInclude.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum customFunctionRequestID
{
    /*===========================================
     * Custom Function Request ID
     * Request ID should be between 0x100 - 0xFFF
     *==========================================*/
    WRITEHELLOWORLD = 0x100,
    READHELLOWORLD = 0x101

}requestIDs;

void customFunctionHookInit();
void customFunctionHook(uint16_T cmdID,uint8_T* payloadBufferRx, uint8_T* payloadBufferTx,uint16_T* peripheralDataSizeResponse);

#ifdef __cplusplus
}
#endif

#endif
