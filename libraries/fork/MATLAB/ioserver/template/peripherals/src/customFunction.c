/* Copyright 2017 The MathWorks, Inc. */
/* Note : If custom function are object oriented in cpp rename the file as customFunction.cpp */
#include "customFunction.h"

static uint8_T Buffer[25];    /* Create a global Buffer to store the data */
/* Init Custom peripherals */
void customFunctionHookInit()
{
    memset(&Buffer,0,25);     /* Init the global Buffer */
}

/* Hook to add the custom peripherals */
void customFunctionHook(uint16_T requestID,uint8_T* payloadBufferRx, uint8_T* payloadBufferTx,uint16_T* peripheralDataSizeResponse)
{

    switch(requestID)
    {
        case WRITEHELLOWORLD :
            memcpy(Buffer,payloadBufferRx,10);   /* Copy the data from received buffer into global Buffer. Assume host has sent 'HelloWorld'. Length('HelloWorld') is 10 bytes */
            break;
        case READHELLOWORLD :
            memcpy(payloadBufferTx,Buffer,10);   /* Copy the data from global Buffer into transmit buffer. Assumed 'HelloWorld' is stored on buffer. Length('HelloWorld') is 10 bytes */
            *peripheralDataSizeResponse = 10;
            break;
        default:

            break;
    }
}


