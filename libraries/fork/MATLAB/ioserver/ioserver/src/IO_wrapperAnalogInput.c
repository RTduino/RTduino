/**
 * @file IO_wrapperAnalogInput.c
 *
 * Wraps around AnalogInput SVD C layer to provide features to access analog input pins through IO Server
 *
 * @Copyright 2017-2019 The MathWorks, Inc.
 *
 */
#include "IO_wrapperAnalogInput.h"
#if IO_STANDARD_ANALOGINPUT
/* The IO_ANALOGINPUT_MODULES_MAX macros will be defined in peripheralIncludes.h */

#ifdef USE_BIT_FOR_HANDLE
/*
 * Creates bits to convey info whether pin is occupied. Bit position is pin no
 * ex - 33 pins.
 * 33/4 = 4
 * 33%4 = 1
 * Provide 5 bytes data to hold pin info
 * if pin 33 is set then
 * byte 5, position 0 is set to 1
 */
PeripheralHandleMapType analogInputMap[(IO_ANALOGINPUT_MODULES_MAX/8)+1] = {0};
#else
PeripheralHandleMapType analogInputMap[IO_ANALOGINPUT_MODULES_MAX] = {NULL};

#endif

#if DEBUG_FLAG == 1

#define CONFSINGLE "ConfigureAnalogInSingle::MW_AnalogInSingle_Open(pin %" PRIu32 ");"
#define CONF "ConfigureAnalogInSingle::MW_AnalogIn_Open(pin %" PRIu32 ", conversiontime %" PRIu32 ");"
#define CONFGROUP "ConfigureAnalogInGroup::MW_AnalogIn_Open(groupID %" PRIu32 ",channels %" PRIu16 ");"
#define SETTRIGGER "SetTriggerSourceAnalogIn::MW_AnalogIn_SetTriggerSource(pin %" PRIu32 ",ttype %s,tvalue %" PRIu32 ");"
#define ENABLENOTIF "EnableNotificationAnalogIn::MW_AnalogIn_EnableNotification(pin %" PRIu32 ");"
#define DISABLENOTIF "DisableNotificationAnalogIn::MW_AnalogIn_DisableNotification(pin %" PRIu32 ");"
#define ENCONTCONV "EnableContConversionAnalogIn::MW_AnalogIn_EnableContConversion(pin %" PRIu32 ");"
#define SETCONVRANK "SetChannelConvRankAnalogIn::MW_AnalogIn_SetChannelConvRank(pin %" PRIu32 ",channel %" PRIu32 ",rank %" PRIu32 ");"
#define GETSTATUS "GetStatusAnalogIn::MW_AnalogIn_GetStatus(pin %" PRIu32 ");"
#define READRESULTSINGLE "ReadResultAnalogInSingle::MW_AnalogInSingle_ReadResult(pin % " PRIu32 ",resulttype %s);"
#define READRESULT "ReadResultAnalogInSingle::MW_AnalogIn_ReadResult(pin % " PRIu32 ",resulttype %s);"
#define READRESULTGRP "ReadResultAnalogInGroup::MW_AnalogIn_ReadResult(gID %" PRIu32 ",channel %" PRIu16 ",resulttype %" PRIu16 ");"
#define STARTCONV "StartAnalogInConversion::MW_AnalogIn_Start(pin %" PRIu32 ");"
#define STOPCONV "StopAnalogInConversion::MW_AnalogIn_Stop(pin %" PRIu32 ");"
#define UNCONF "UnconfigureAnalogInSingle::MW_AnalogIn_Close(pin %" PRIu32 ");"
#define RESETANALOG1 "ResetAnalogPins::MW_AnalogIn_Stop(pin %" PRIu16 ");"
#define RESETANALOG2 "ResetAnalogPins::MW_AnalogIn_Close(pin %" PRIu16 ");"

const char MSG_CONF_ANALOGINSINGLE[] PGMKEYWORD = CONFSINGLE;
const char MSG_CONF_ANALOGIN[] PGMKEYWORD = CONF;
const char MSG_CONF_ANALOGINGROUP[] PGMKEYWORD = CONFGROUP;
const char MSG_SETTRIGGER[] PGMKEYWORD = SETTRIGGER;
const char MSG_ENABLENOTIF[] PGMKEYWORD = ENABLENOTIF;
const char MSG_DISABLENOTIF[] PGMKEYWORD = DISABLENOTIF;
const char MSG_ENABLECONTCONV[] PGMKEYWORD = ENCONTCONV;
const char MSG_SETCHANNELCONVRANK[] PGMKEYWORD = SETCONVRANK;
const char MSG_GETSTATUS[] PGMKEYWORD = GETSTATUS;
const char MSG_READRESULTSINGLE[] PGMKEYWORD = READRESULTSINGLE;
const char MSG_READRESULT[] PGMKEYWORD = READRESULT;
const char MSG_READRESULTGROUP[] PGMKEYWORD = READRESULTGRP;
const char MSG_STARTANALOGINPCONV[] PGMKEYWORD = STARTCONV;
const char MSG_STOPANALOGINPCONV[] PGMKEYWORD = STOPCONV;
const char MSG_UNCONF[] PGMKEYWORD = UNCONF;
const char MSG_RESET_ANALOG_STOP[] PGMKEYWORD = RESETANALOG1;
const char MSG_RESET_ANALOG_CLOSE[] PGMKEYWORD = RESETANALOG2;
const char resultType[NUMRESULTTYPES][MAXRESULTTYPELENGTH] PGMKEYWORD = {"int8","uint8","int16","uint16","int32","uint32","float","double"};

#endif


/***************************************************************************************************************************
 *              Configures the specified analog input pin.
 *              Returns status as 0 after successful open.
 *              Returns status as 1 on failure.
 ****************************************************************************************************************************/
void configureAnalogInSingle(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
#if ESB_BETA2_IOSERVER
    uint32_T conversionTimeMicSecs = 0;
#endif
    MW_Handle_Type analogInputHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);
#if ESB_BETA2_IOSERVER
    /* Reads 4 byte conversion time micro secs from the input buffer */
    memcpy(&conversionTimeMicSecs, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);
#if DEBUG_FLAG == 1
    /* Prints a debug message */
    debugPrint(MSG_CONF_ANALOGIN,pinNo, (uint32_T) converstionTimeMicSecs);
#endif

    analogInputHandle = (MW_Handle_Type)MW_AnalogIn_Open((uint32_T) pinNo, (uint32_T) conversionTimeMicSecs);
#else

#if DEBUG_FLAG == 1
    /* Prints a debug message */
    debugPrint(MSG_CONF_ANALOGINSINGLE,pinNo);
#endif

    analogInputHandle = (MW_Handle_Type)MW_AnalogInSingle_Open((uint32_T)pinNo);
#endif
    if ((MW_Handle_Type)NULL != analogInputHandle) {
        /* Stores the handle if open was successful */
        setHandle(pinNo, analogInputHandle, analogInputMap);

        /* Returns status as 0 if open is successful */
        *status = 0;
    } else {
        /* Returns status as 1 if open is not successful */
        *status = 1;
    }

}

/***************************************************************************************************************************
 *              Create analog input group with channels and conversion time.
 *              Returns status as 0.
 ****************************************************************************************************************************/
void configureAnalogInGroup(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
#if 0
    uint8_T index = 0;
    uint8_T numberOfChannels = 0;
    uint32_T groupId = 0;
    uint32_T * channelsId_ptr = NULL;
    real32_T * convTimeInSecs_ptr = NULL;

    /* Reads 4 byte group ID from the input buffer */
    memcpy(&groupId, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Reads 1 byte number of channel from the input buffer */
    memcpy(&numberOfChannels, &payloadBufferRx[index], sizeof(uint8_T));
    index += sizeof(uint8_T);

    /* Adjust the pointer to point array of analog input channels */
    channelsId_ptr = (uint32_T *)&payloadBufferRx[index];
    index = index + (numberOfChannels * sizeof(uint32_T));

    /* Adjust the pointer to point conversion time for analog input channels */
    convTimeInSecs_ptr = (real32_T *)&payloadBufferRx[index];
    index = index + (numberOfChannels * sizeof(real32_T));

#if DEBUG_FLAG == 1
    /* Prints a debug message */
    debugPrint(MSG_CONF_ANALOGINGROUP,(uint32_T)groupId,(uint8_T) numberOfChannels);
#endif

    MW_AnalogIn_Open((uint32_T)groupId, (uint32_T *) channelsId_ptr, (real32_T *) convTimeInSecs_ptr, (uint8_T) numberOfChannels);

    /* Always passes because SVD doesn't return a status */
    *status = 0;
#endif

}

/***************************************************************************************************************************
 *              Select trigger source for AnalogIn group to start conversion.
 *              Returns status as 0 if analog input pin is already configured.
 *              Returns status as 1 if analog input pin is not configured.
 ****************************************************************************************************************************/
void setTriggerSourceAnalogIn(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    int8_T triggerType = 0;
    uint32_T triggerValue = 0;
    MW_Handle_Type analogInputHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Reads 1 byte trigger type from the input buffer */
    memcpy(&triggerType, &payloadBufferRx[index], sizeof(int8_T));
    index += sizeof(int8_T);

    /* Reads 4 byte trigger value from the input buffer */
    memcpy(&triggerValue, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Retrieve the analog input pin handle from the corresponding handle map */
    analogInputHandle = getHandle(pinNo, analogInputMap);

    if ((MW_Handle_Type)NULL != analogInputHandle)
    {
        MW_AnalogIn_SetTriggerSource((MW_Handle_Type)analogInputHandle,(MW_AnalogIn_TriggerSource_Type) triggerType,(uint32_T) triggerValue);

        if (1 == triggerType)
        {
#if DEBUG_FLAG == 1
            /* Prints a debug message */
            debugPrint(MSG_SETTRIGGER,(uint32_T)pinNo,"External trigger",(uint32_T) triggerValue);
#endif
        }
        else
        {
#if DEBUG_FLAG == 1
            /* Prints a debug message */
            debugPrint(MSG_SETTRIGGER,(uint32_T)pinNo,"Software",(uint32_T) triggerValue);
#endif
        }

        /* Always passes because SVD doesn't return status */
        *status = 0;
    }
    else
    {
        *status = 1;
    }
}

/***************************************************************************************************************************
 *              Enable Conversion complete notification.
 *              Returns status as 0 if analog input pin is already configured.
 *              Returns status as 1 if analog input pin is not configured.
 ****************************************************************************************************************************/
void enableNotificationAnalogIn(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    MW_Handle_Type analogInputHandle = NULL;
#if ESB_BETA2_IOSERVER
    uint32_T notificationValue = 0;
#endif

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Retrieve the analog input pin handle from the corresponding handle map */
    analogInputHandle = getHandle(pinNo, analogInputMap);

    if ((MW_Handle_Type)NULL != analogInputHandle)
    {
#if DEBUG_FLAG == 1
        /* Prints a debug message */
        debugPrint(MSG_ENABLENOTIF,pinNo);
#endif
#if ESB_BETA2_IOSERVER
        /* Reads 4 byte notification value from the input buffer */
        memcpy(&notificationValue, &payloadBufferRx[index], sizeof(uint32_T));
        index += sizeof(uint32_T);
        MW_AnalogIn_EnableNotification((MW_Handle_Type)analogInputHandle, (uint32_T) notificationValue);
#else
        MW_AnalogIn_EnableNotification((MW_Handle_Type)analogInputHandle);
#endif


        /* Always passes because SVD doesn't return status */
        *status = 0;
    } else {
        *status = 1;
    }
}

/***************************************************************************************************************************
 *              Disable Conversion complete notification.
 *              Returns status as 0 if analog input pin is already configured.
 *              Returns status as 1 if analog input pin is not configured.
 ****************************************************************************************************************************/
void disableNotificationAnalogIn(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    MW_Handle_Type analogInputHandle = NULL;
#if ESB_BETA2_IOSERVER
    uint32_T notificationValue = 0;
#endif

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Retrieve the analog input pin handle from the corresponding handle map */
    analogInputHandle = getHandle(pinNo, analogInputMap);

    if ((MW_Handle_Type)NULL != analogInputHandle)
    {
#if DEBUG_FLAG == 1
        /* Prints a debug message */
        debugPrint(MSG_DISABLENOTIF,pinNo);
#endif
#if ESB_BETA2_IOSERVER
        /* Reads 4 byte notification value from the input buffer */
        memcpy(&notificationValue, &payloadBufferRx[index], sizeof(uint32_T));
        index += sizeof(uint32_T);
        MW_AnalogIn_DisableNotification((MW_Handle_Type)analogInputHandle, (uint32_T) notificationValue);
#else
        MW_AnalogIn_DisableNotification((MW_Handle_Type)analogInputHandle);
#endif


        /* Always passes because SVD doesn't return status */
        *status = 0;
    } else {
        *status = 1;
    }
}

/***************************************************************************************************************************
 *              Enable continuous conversion.
 *              Returns status as 0 if analog input pin is already configured.
 *              Returns status as 1 if analog input pin is not configured.
 ****************************************************************************************************************************/
void enableContConversionAnalogIn(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    MW_Handle_Type analogInputHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Retrieve the analog input pin handle from the corresponding handle map */
    analogInputHandle = getHandle(pinNo, analogInputMap);
    if ((MW_Handle_Type)NULL != analogInputHandle)
    {
#ifndef ESB_BETA2_IOSERVER

#if DEBUG_FLAG == 1
        /* Prints a debug message */
        debugPrint(MSG_ENABLECONTCONV,pinNo);
#endif

        MW_AnalogIn_EnableContConversion((MW_Handle_Type)analogInputHandle);
#endif
        /* Always passes because SVD doesn't return status */
        *status = 0;
    }
    else
    {
        *status = 1;
    }
}

/***************************************************************************************************************************
 *              Set channel conversion priority.
 *              Returns status as 0 if analog input pin is already configured.
 *              Returns status as 1 if analog input pin is not configured.
 ****************************************************************************************************************************/
void setChannelConvRankAnalogIn(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
#if 0
    uint8_T index = 0;
    uint32_T pinNo = 0;
    uint32_T channel = 0;
    uint32_T rank = 0;
    MW_Handle_Type analogInputHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Reads 4 byte channel from the input buffer */
    memcpy(&channel, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Reads 4 byte priority from the input buffer */
    memcpy(&rank, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Retrieve the analog input pin handle from the corresponding handle map */
    analogInputHandle = getHandle(pinNo, analogInputMap);

    if ((MW_Handle_Type)NULL != analogInputHandle)
    {
#if DEBUG_FLAG == 1
        /* Prints a debug message */
        debugPrint(MSG_SETCHANNELCONVRANK,pinNo,(uint32_T) channel, (uint32_T) rank);
#endif
        MW_AnalogIn_SetChannelConvRank((MW_Handle_Type)analogInputHandle, (uint32_T) channel, (uint32_T) rank);

        /* Always passes because SVD doesn't return status */
        *status = 0;
    }
    else
    {
        *status = 1;
    }
#endif
}

/***************************************************************************************************************************
 *              Get status of analog input pin.
 *              Returns analog input pin status if already configured.
 *              Returns status as 0 if analog input pin is already configured.
 *              Returns status as 1 if analog input pin is not configured.
 ****************************************************************************************************************************/
void getStatusAnalogIn(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{

    uint8_T index = 0;
    uint8_T adcStatus = 0;
    uint32_T pinNo = 0;
    MW_Handle_Type analogInputHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);


    /* Retrieve the analog input pin handle from the corresponding handle map */
    analogInputHandle = getHandle(pinNo, analogInputMap);

    if ((MW_Handle_Type)NULL != analogInputHandle)
    {
        adcStatus = (uint8_T)MW_AnalogIn_GetStatus((MW_Handle_Type)analogInputHandle);

#if DEBUG_FLAG == 1
        /* Prints a debug message */
        debugPrint(MSG_GETSTATUS,pinNo);
#endif
        /* Transmits the status of analog pin back to the host */
        payloadBufferTx[(*peripheralDataSizeResponse)++] = (uint8_T)adcStatus;
        /* Returns status as 0 pin is configured as analog input */
        *status = 0;
    } else {
        /* Returns status as 1 pin is not configured as analog input */
        *status = 1;
    }
}


/***************************************************************************************************************************
 *              Read single analog input conversion result.
 *              Returns result if analog input pin is already configured.
 *              Returns status as 0 if analog input pin is already configured.
 *              Returns status as 1 if analog input pin is not configured.
 ****************************************************************************************************************************/
void readResultAnalogInSingle(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{

    uint8_T index = 0;
    uint8_T resultDataType = 0;
    uint8_T responsePeripheralPayloadSize = 0;
    uint32_T pinNo = 0;
    MW_Handle_Type analogInputHandle = NULL;
#if DEBUG_FLAG == 1
#ifdef __AVR_ARCH__
    int i;
    char rType[MAXRESULTTYPELENGTH];
#endif
#endif
    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Reads 1 byte result data type from the input buffer */
    memcpy(&resultDataType, &payloadBufferRx[index], sizeof(uint8_T));
    index += sizeof(uint8_T);

    /* Reads 1 byte response peripheral payloadSize from the input buffer */
    memcpy(&responsePeripheralPayloadSize, &payloadBufferRx[index], sizeof(uint8_T));
    index += sizeof(uint8_T);

    /* Retrieve the analog input pin handle from the corresponding handle map */
    analogInputHandle = getHandle(pinNo, analogInputMap);

    if ((MW_Handle_Type)NULL != analogInputHandle) {
#if DEBUG_FLAG == 1
#ifdef __AVR_ARCH__
        getProgramMemoryData(resultType[resultDataType], rType);
#if ESB_BETA2_IOSERVER
        debugPrint(MSG_READRESULT,(uint32_T)pinNo,rType);
#else
        debugPrint(MSG_READRESULTSINGLE,(uint32_T)pinNo,rType);
#endif
#else
#if ESB_BETA2_IOSERVER
        /* Prints a debug message */
        debugPrint(MSG_READRESULT,(uint32_T)pinNo,resultType[resultDataType]);
#else
        debugPrint(MSG_READRESULTSINGLE,(uint32_T)pinNo,resultType[resultDataType]);
#endif
#endif
#endif

#if ESB_BETA2_IOSERVER
        MW_AnalogIn_ReadResult((MW_Handle_Type)analogInputHandle, (void *)payloadBufferTx, (MW_AnalogIn_ResultDataType_Type)resultDataType);
#else
        MW_AnalogInSingle_ReadResult((MW_Handle_Type)analogInputHandle, (void *)payloadBufferTx, (MW_AnalogIn_ResultDataType_Type)resultDataType);
#endif
        /* Transmits the result of analog pin back to the host */
        *peripheralDataSizeResponse = (uint16_T)responsePeripheralPayloadSize;
        /* Returns status as 0 pin is configured as analog input */
        *status = 0;
    } else {
        /* Returns status as 1 pin is not configured as analog input */
        *status = 1;
    }
}


/***************************************************************************************************************************
 *              Read group analog input conversion result.
 *              Returns result if analog input pin is already configured.
 *              Returns status as 0.
 ****************************************************************************************************************************/
void readResultAnalogInGroup(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
#if 0
    uint8_T index = 0;
    uint8_T resultDataType = 0;
    uint8_T numberOfChannels = 0;
    uint16_T responsePeripheralPayloadSize = 0;
    uint32_T groupId = 0;

    /* Reads 4 byte group Id from the input buffer */
    memcpy(&groupId, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Reads 1 number of channel from the input buffer */
    memcpy(&numberOfChannels, &payloadBufferRx[index], sizeof(uint8_T));
    index += sizeof(uint8_T);

    /* Reads 1 byte result data type from the input buffer */
    memcpy(&resultDataType, &payloadBufferRx[index], sizeof(uint8_T));
    index += sizeof(uint8_T);

    /* Reads 2 byte response peripheral payloadSize from the input buffer */
    memcpy(&responsePeripheralPayloadSize, &payloadBufferRx[index], sizeof(uint16_T));
    index += sizeof(uint16_T);

#if DEBUG_FLAG == 1
    /* Prints a debug message */
    debugPrint(MSG_READRESULTGROUP,(uint32_T) groupId, (uint8_T) numberOfChannels, (uint8_T)resultDataType);
#endif

    MW_AnalogIn_ReadResult((uint32_T) groupId, (void *) payloadBufferTx, (uint8_T) numberOfChannels, (MW_AnalogIn_ResultDataType_Type) resultDataType);

    /* Transmits the result of analog pin back to the host */
    *peripheralDataSizeResponse = (uint16_T)responsePeripheralPayloadSize;

    /* Always passes because SVD doesn't return a status */
    *status = 0;
#endif
}

/***************************************************************************************************************************
 *              Start analog input conversion.
 *              Returns status as 0 if analog input pin is already configured.
 *              Returns status as 1 if analog input pin is not configured.
 ****************************************************************************************************************************/
void startAnalogInConversion(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    MW_Handle_Type analogInputHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Retrieve the analog input pin handle from the corresponding handle map */
    analogInputHandle = getHandle(pinNo, analogInputMap);

    if ((MW_Handle_Type)NULL != analogInputHandle) {

#if DEBUG_FLAG == 1
        /* Prints a debug message */
        debugPrint(MSG_STARTANALOGINPCONV,pinNo);
#endif
        MW_AnalogIn_Start((MW_Handle_Type)analogInputHandle);
        /* Always passes because SVD doesn't return status */
        /* Returns status as 0 pin is configured as analog input */
        *status = 0;
    } else {
        /* Returns status as 1 pin is not configured as analog input */
        *status = 1;
    }
}


/***************************************************************************************************************************
 *              Stop analog input conversion.
 *              Returns status as 0 if analog input pin is already configured.
 *              Returns status as 1 if analog input pin is not configured.
 ****************************************************************************************************************************/
void stopAnalogInConversion(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    MW_Handle_Type analogInputHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Retrieve the analog input pin handle from the corresponding handle map */
    analogInputHandle = getHandle(pinNo, analogInputMap);

    if ((MW_Handle_Type)NULL != analogInputHandle) {

#if DEBUG_FLAG == 1
        /* Prints a debug message */
        debugPrint(MSG_STOPANALOGINPCONV,pinNo);
#endif
        MW_AnalogIn_Stop((MW_Handle_Type)analogInputHandle);
        /* Always passes because SVD doesn't return status */
        /* Returns status as 0 pin is configured as analog input */
        *status = 0;
    } else {
        /* Returns status as 1 pin is not configured as analog input */
        *status = 1;
    }
}


/***************************************************************************************************************************
 *              Reset analog input pins.
 *              Returns status as 0.
 ****************************************************************************************************************************/
void resetAnalogPins(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint16_T loopvar = 0;
    MW_Handle_Type IOHandle = NULL;

    for (loopvar = 0 ; loopvar < IO_ANALOGINPUT_MODULES_MAX ; loopvar++)
    {
        IOHandle = getHandle(loopvar, analogInputMap);
        if ((MW_Handle_Type)NULL != IOHandle)
        {
            /* close the analog pin */
#if DEBUG_FLAG == 1
            /* print debug message */
            debugPrint(MSG_RESET_ANALOG_STOP,loopvar);
#endif

            MW_AnalogIn_Stop((MW_Handle_Type)IOHandle);

#if DEBUG_FLAG == 1
            /* print debug message */
            debugPrint(MSG_RESET_ANALOG_CLOSE,loopvar);
#endif

            MW_AnalogIn_Close((MW_Handle_Type)IOHandle);
            setHandle(loopvar, (MW_Handle_Type)NULL, analogInputMap);
        }
    }
    /* Returns status as 0 */
    *status = 0;
}

/***************************************************************************************************************************
 *              Unconfigure analog input pin.
 *              Returns status as 0 if pin is unconfigured.
 *              Returns status as 1 if pin was not never configured for analog input operation.
 ****************************************************************************************************************************/
void unconfigureAnalogInSingle(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    MW_Handle_Type analogInputHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Retrieve the analog input pin handle from the corresponding handle map */
    analogInputHandle = getHandle(pinNo, analogInputMap);

    if ((MW_Handle_Type)NULL != analogInputHandle)
    {

#if DEBUG_FLAG == 1
        /* Prints a debug message */
        debugPrint(MSG_UNCONF,pinNo);
#endif

        MW_AnalogIn_Close((MW_Handle_Type)analogInputHandle);
        setHandle(pinNo, (MW_Handle_Type)NULL, analogInputMap);
        /* Always passes because SVD doesn't return status */
        /* Returns status as 0 if pin was configured for analog input operation */
        *status = 0;
    }
    else
    {
        /* Returns status as 1 if pin was not configured for analog input operation */
        *status = 1;
    }
}
#endif
