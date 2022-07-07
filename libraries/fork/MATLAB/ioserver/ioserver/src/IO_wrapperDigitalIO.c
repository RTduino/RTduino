/**
 * @file IO_wrapperDigitalIO.c
 *
 * Wraps around digitalIO SVD C layer to provide features to access Digital pins through IO Server
 *
 * @Copyright 2017-2018 The MathWorks, Inc.
 *
 */
#include "IO_wrapperDigitalIO.h"
#if IO_STANDARD_DIGITALIO
/* The IO_DIGITALIO_MODULES_MAX macros will be defined in peripheralIncludes.h */
#ifdef USE_BIT_FOR_HANDLE
/*
Creates bits to convey info whether pin is occupied. Bit position is pin no
ex - 33 pins.
33/4 = 4
33%4 = 1
Provide 5 bytes data to hold pin info
if pin 33 is set then
byte 5, position 0 is set to 1
*/
PeripheralHandleMapType digitalIOMap[(IO_DIGITALIO_MODULES_MAX/8)+1] = {0};
#else
PeripheralHandleMapType digitalIOMap[IO_DIGITALIO_MODULES_MAX] = {NULL};
#endif

#if DEBUG_FLAG == 1

#define CONFPIN "ConfigureDigitalPin::MW_digitalIO_open(pin %" PRIu32 ",direction %" PRIu16 ");"
#define WRITEPIN "WriteDigitalPin::MW_digitalIO_write(pin %" PRIu32 ",value %" PRIu16 ");"
#define READPIN "ReadDigitalPin::MW_digitalIO_read(pin %" PRIu32 ");"
#define UNCONFPIN "UnconfigureDigitalPin::MW_digitalIO_close(pin %" PRIu32 ");"
#define RESETPIN "ResetDigitalPins::MW_digitalIO_close(pin %" PRIu16 ");"

const char MSG_CONFIG_DIGITALPIN[] PGMKEYWORD = CONFPIN;
const char MSG_WRITE_DIGITALPIN[] PGMKEYWORD = WRITEPIN;
const char MSG_READ_DIGITALPIN[] PGMKEYWORD = READPIN;
const char MSG_UNCONFIG_DIGITALPIN[] PGMKEYWORD = UNCONFPIN;
const char MSG_RESET_DIGITAL[] PGMKEYWORD = RESETPIN;

#endif

/***************************************************************************************************************************
 *              Opens the specified digital pin with the specified direction.
 *              Returns status as 0 after successful open.
 *              Returns status as 1 on failure.
 ****************************************************************************************************************************/
void configureDigitalPin(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    uint8_T direction = 0;
    MW_Handle_Type digitalIOHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Reads 1 byte pin direction from the input buffer */
    direction = payloadBufferRx[index++];

    #if DEBUG_FLAG == 1
        /* Prints a debug message */
        debugPrint(MSG_CONFIG_DIGITALPIN,(uint32_T)pinNo,(uint8_T)direction);
    #endif

#if ESB_BETA2_IOSERVER
    digitalIOHandle = MW_DigitalIO_Open((uint32_T)pinNo, (MW_DigitalIO_Direction_T)direction);
#else
    digitalIOHandle = MW_digitalIO_open((uint32_T)pinNo, (uint8_T)direction);
#endif
    if ((MW_Handle_Type)NULL != digitalIOHandle)
    {
        /* Stores the handle if open was successful */
        setHandle(pinNo, digitalIOHandle, digitalIOMap);

        /* Returns status as 0 if open is successful */
        *status = 0;
    }
    else
    {
        /* Returns status as 1 if open is not successful */
        *status = 1;
    }
}

/***************************************************************************************************************************
 *              Sets or clears the specified digital pin.
 *              Returns status as 0 if pin is configured for digital operation.
 *              Returns status as 1 if pin is not configured for digital operation.
 ****************************************************************************************************************************/

void writeDigitalPin(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    MW_Handle_Type digitalIOHandle = NULL;
    boolean_T pinValue = false;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Reads 1 byte value from the input buffer */
    pinValue = (boolean_T)payloadBufferRx[index++];

    /* Retrieve the digital pin handle from the corresponding handle map */
    digitalIOHandle = getHandle(pinNo, digitalIOMap);

    if ((MW_Handle_Type)NULL != digitalIOHandle)
    {


     #if DEBUG_FLAG == 1
        /* Prints a debug message */
        debugPrint(MSG_WRITE_DIGITALPIN,pinNo,pinValue);
     #endif

#if ESB_BETA2_IOSERVER
        MW_DigitalIO_Write(digitalIOHandle, pinValue);
#else
        MW_digitalIO_write(digitalIOHandle, pinValue);
#endif
        /* Returns status as 0 if pin is configured for digital operation */
        *status = 0;
    }
    else
    {
        /* Returns status as 1 if pin is configured for digital operation */
        *status = 1;
    }
}

/***************************************************************************************************************************
 *              Reads status of the specified digital pin.
 *              Returns 1 if pin is high and status as 0 if configured for digital operation.
 *              Returns 0 if pin is low and status as 0 configured for digital operation.
 *              Returns status as 1 pin is not configured as digital input.
 ****************************************************************************************************************************/
void readDigitalPin(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{

    uint8_T index=0;
    boolean_T pinValue=0;
    uint32_T pinNo = 0;
    MW_Handle_Type digitalIOHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Retrieve the digital pin handle from the corresponding handle map */
    digitalIOHandle = getHandle(pinNo, digitalIOMap);

    if ((MW_Handle_Type)NULL != digitalIOHandle)
    {

      #if DEBUG_FLAG == 1
        /* Prints a debug message */
        debugPrint(MSG_READ_DIGITALPIN,pinNo);
      #endif

#if ESB_BETA2_IOSERVER
        pinValue = MW_DigitalIO_Read(digitalIOHandle);
#else
        pinValue = MW_digitalIO_read(digitalIOHandle);
#endif
        /* Returns status as 0 pin is configured for digital operation */
        *status = 0;
    }
    else
    {
        /* Returns status as 1 pin is not configured for digital operation */
        *status = 1;
    }
        /* Transmits the status of digital pin back to the host */
        payloadBufferTx[(*peripheralDataSizeResponse)++] = (uint8_T)pinValue;
}

/***************************************************************************************************************************
 *              Unconfigures the specified digital pin.
 *              Returns status as 0 if pin is unconfigured.
 *              Returns status as 1 if pin was not never configured for digital operation.
 ****************************************************************************************************************************/
void unconfigureDigitalPin(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    MW_Handle_Type digitalIOHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Retrieve the digital pin handle from the corresponding handle map */
    digitalIOHandle = getHandle(pinNo, digitalIOMap);

     if ((MW_Handle_Type)NULL != digitalIOHandle)
     {
       #if DEBUG_FLAG == 1
        /* Prints a debug message */
        debugPrint(MSG_UNCONFIG_DIGITALPIN,pinNo);
       #endif
        /* close the digital pin */
        #if ESB_BETA2_IOSERVER
            MW_DigitalIO_Close(digitalIOHandle);
        #else
            MW_digitalIO_close(digitalIOHandle);
        #endif
        setHandle(pinNo, (MW_Handle_Type)NULL, digitalIOMap);
        /* Returns status as 0 if pin was configured for digital operation */
        *status = 0;
     }
    else
    {
        /* Returns status as 1 if pin was not configured for digital operation */
        *status = 1;
    }
}

/***************************************************************************************************************************
 *              Reset digital pins.
 *              Returns status as 0
 ****************************************************************************************************************************/
void resetDigitalPins(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint16_T loopvar = 0;
    MW_Handle_Type IOHandle = NULL;

    for (loopvar = 0 ; loopvar < IO_DIGITALIO_MODULES_MAX ; loopvar++)
    {
        IOHandle = getHandle(loopvar, digitalIOMap);
        if ((MW_Handle_Type)NULL != IOHandle)
        {

            #if DEBUG_FLAG == 1
                /* print debug message */
                debugPrint(MSG_RESET_DIGITAL,loopvar);
            #endif

            /* close the digital pin */
#if ESB_BETA2_IOSERVER
            MW_DigitalIO_Close(IOHandle);
#else
            MW_digitalIO_close(IOHandle);
#endif
            setHandle(loopvar, (MW_Handle_Type)NULL, digitalIOMap);
        }
    }
    /* Returns status as 0 */
    *status = 0;
}
#endif
