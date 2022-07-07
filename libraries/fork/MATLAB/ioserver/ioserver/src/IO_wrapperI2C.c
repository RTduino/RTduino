/**
 * @file IO_wrapperI2C.c
 *
 * Wraps around I2C SVD C layer to provide features to access I2C bus through IO Server
 *
 * @Copyright 2017-2018 The MathWorks, Inc.
 *
 */


#include "IO_wrapperI2C.h"
#if IO_STANDARD_I2C
static const uint16_T MINIMUM_USABLE_7BIT_I2CADDRESS_08 = 0x08;
static const uint16_T MAXIMUM_USABLE_7BIT_I2CADDRESS_119 = 0x77;
static const uint8_T REPEATED_START = 1;
static const uint8_T NO_REPEATED_START = 0;
static const uint8_T ACKNOWLEDGE_BYTE = 1;
static const uint8_T DONT_ACKNOWLEDGE_BYTE = 0;

#if DEBUG_FLAG == 1

#define SCANBUS "ScanI2CBus::MW_I2C_Open(i2cBus %" PRIu32 ");"
#define RAWWRITE "RawI2CWrite::MW_I2C_MasterWrite(i2cBus %" PRIu32 ",slaveaddress %" PRIu16 ");"
#define RAWWRITE_1 "RawI2CWrite::MW_I2C_MasterWrite(data %" PRIu16 ");"
#define RAWREAD "RawI2CRead::MW_I2C_MasterRead(i2cBus %" PRIu32 ",slaveaddress %" PRIu16 ",bytes2read %" PRIu32 ");"
#define REGI2CWRITE "RegisterI2CWrite::MW_I2C_MasterWrite(i2cBus %" PRIu32 ",slaveaddress %" PRIu16 ",registeraddress %" PRIu16 ");"
#define REGI2CWRITE_1 "RegisterI2CWrite::MW_I2C_MasterWrite(data %" PRIu16 ");"
#define REGI2CREAD_1 "RegisterI2CRead::MW_I2C_MasterWrite(i2cBus %" PRIu32 ",slaveaddress %" PRIu16 ",registeraddress %" PRIu16 ");"
#define REGI2CREAD_2 "RegisterI2CRead::MW_I2C_MasterRead(i2cBus %" PRIu32 ",slaveaddress %" PRIu16 ",bytes2read %" PRIu16 ");"
#define SETI2CFREQ "SetI2CFrequency::MW_I2C_SetBusSpeed(i2cBus %" PRIu32",frequency %" PRIu32 ");"
#define OPENI2CBUS1 "OpenI2CBus::MW_I2C_Open(i2cBus %" PRIu32");"
#define OPENI2CBUS2 "OpenI2CBus::MW_I2C_SetBusSpeed(frequency %" PRIu32");"
#define CLOSEI2CBUS "CloseI2CBus::MW_I2C_Close(i2cBus %" PRIu32");"

const char MSG_SCAN_I2CBUS[] PGMKEYWORD = SCANBUS;
const char MSG_RAW_I2CWRITE[] PGMKEYWORD = RAWWRITE;
const char MSG_RAW_I2CWRITE1[] PGMKEYWORD = RAWWRITE_1;
const char MSG_RAW_I2CREAD[] PGMKEYWORD = RAWREAD;
const char MSG_REG_I2CWRITE[] PGMKEYWORD = REGI2CWRITE;
const char MSG_REG_I2CWRITE1[] PGMKEYWORD = REGI2CWRITE_1;
const char MSG_REG_I2CREAD1[] PGMKEYWORD = REGI2CREAD_1;
const char MSG_REG_I2CREAD2[] PGMKEYWORD = REGI2CREAD_2;
const char MSG_SET_I2CFREQ[] PGMKEYWORD = SETI2CFREQ;
const char MSG_OPENI2CBUS1[] PGMKEYWORD = OPENI2CBUS1;
const char MSG_OPENI2CBUS2[] PGMKEYWORD = OPENI2CBUS2;
const char MSG_CLOSEI2CBUS[] PGMKEYWORD = CLOSEI2CBUS;

#endif

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
PeripheralHandleMapType i2cMap[(IO_I2C_MODULES_MAX/8)+1] = {0};
#else
PeripheralHandleMapType i2cMap[IO_I2C_MODULES_MAX] = {NULL};

#endif
/* The IO_I2C_MODULES_MAX macros will be defined in peripheralIncludes.h */


/***************************************************************************************************************************
 *       Scans the I2C bus for I2C devices
 *       Returns status as 4 (MW_I2C_NO_ACKNOWLEDGE) on failure
 *       Returns status as 0 (MW_I2C_SUCCESS) slave addresses on success
 ****************************************************************************************************************************/
void scanI2CBus(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{
    uint16_T slaveAddress = MINIMUM_USABLE_7BIT_I2CADDRESS_08;
    boolean_T _7_BIT_ADDRESSING = 0;
    boolean_T _10_BIT_ADDRESSING = 0;
    boolean_T handleAvailable = 0;
    uint16_T index = 0;
    uint8_T data = 0;
    uint32_T dataLength = 0;
    MW_Handle_Type i2cHandle;

    /* Set status as no acknowledge by default */
    *status = MW_I2C_NO_ACKNOWLEDGE;

    /* Reads 1 byte bus number from the input buffer */
    uint32_T i2cBus = payloadBufferRx[index++];
    /* Reads the addressing mode from the input buffer */
    if (payloadBufferRx[index++]) {
        _7_BIT_ADDRESSING = true;
    } else {
        _10_BIT_ADDRESSING = true;
    }

    /* Retrieve the I2C bus handle from the corresponding handle map */
    i2cHandle = getHandle(i2cBus, i2cMap);
    /* Maintans a flag to check if the bus was open before entering into this function */
    if (NULL != i2cHandle) {
        handleAvailable = true;
    } else {
        handleAvailable = false;
        /* Opens the bus since it was closed */
        i2cHandle = MW_I2C_Open(i2cBus, MW_I2C_MASTER);

#if DEBUG_FLAG == 1
        /* print debug message */
        debugPrint(MSG_SCAN_I2CBUS,i2cBus);
#endif

        setHandle(i2cBus, i2cHandle, i2cMap);
    }
    if ((MW_Handle_Type)NULL != i2cHandle) {
        if (_7_BIT_ADDRESSING) {
            /* Loops through all the devices to be connected */
            while (slaveAddress <= MAXIMUM_USABLE_7BIT_I2CADDRESS_119) {
                /* Writes a dummy data to check if the device is connected. (Just checks slave address ACK) */
                *status = MW_I2C_MasterWrite(i2cHandle, slaveAddress, &data, dataLength, REPEATED_START, DONT_ACKNOWLEDGE_BYTE);
                if (MW_I2C_SUCCESS == *status)
                {
                    /* Stores the device address in the transmit buffer */
                    payloadBufferTx[(*peripheralDataSizeResponse)++] = (uint8_T)slaveAddress;
                }

                slaveAddress++;
            }
        } else if (_10_BIT_ADDRESSING) {
            /* Space for 10-bit addressing implementation */
        }
        /* Closes the bus if the bus was not open before entering this function */
        if (!handleAvailable) {
            /* close the I2C bus */
            MW_I2C_Close(i2cHandle);
            setHandle(i2cBus, (MW_Handle_Type)NULL, i2cMap);
        }
        *status = MW_I2C_SUCCESS;
    }
}

/************************************************************************************************************
 *       Raw write into the I2C bus.
 *       Returns status given by MW_I2C_MasterWrite SVD function.
 ************************************************************************************************************/
void rawI2CWrite(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{
    MW_Handle_Type i2cHandle;
    uint16_T index = 0;
    uint32_T numBytesToWrite = 0;
    uint16_T slaveAddress = 0;
    uint8_T *data = NULL;
    /* Set status as no acknowledge by default */
    *status = MW_I2C_NO_ACKNOWLEDGE;

#if DEBUG_FLAG == 1
    uint8_T tempVar = 0;
#endif
    /* Reads 1 byte bus number from the input buffer */
    uint32_T i2cBus = payloadBufferRx[index];
    index++;

    /* Reads 2 byte Slave address from the input buffer */
    slaveAddress = (uint8_T)payloadBufferRx[index] | ((uint8_T)payloadBufferRx[index+1]) << 8;
    index += 2;

    /* Reads 4 byte data length from the input buffer */
    numBytesToWrite = (uint32_T)payloadBufferRx[index] | ((uint32_T)payloadBufferRx[index+1]) << 8 | ((uint32_T)payloadBufferRx[index+2]) << 16 | ((uint32_T)payloadBufferRx[index+3]) << 24;
    index += 4;

    /* Copies the pointer to the data to be written (rest of the input buffer) */
    data = (uint8_T*)&payloadBufferRx[index];

    /* Retrieve the I2C bus handle from the corresponding handle map */
    i2cHandle = getHandle(i2cBus, i2cMap);

    if ((MW_Handle_Type)NULL != i2cHandle)
    {
        /* Writes the data to the slave in 7-bit addressing format */
        if (slaveAddress >= MINIMUM_USABLE_7BIT_I2CADDRESS_08 && slaveAddress <= MAXIMUM_USABLE_7BIT_I2CADDRESS_119)
        {
#if DEBUG_FLAG == 1
            /* print debug message */
            debugPrint(MSG_RAW_I2CWRITE, i2cBus, slaveAddress);
            for(tempVar = 0; tempVar < numBytesToWrite; tempVar++)
            {
                debugPrint(MSG_RAW_I2CWRITE1, *(data+tempVar));
            }
#endif
            *status = MW_I2C_MasterWrite(i2cHandle, slaveAddress, data, numBytesToWrite, NO_REPEATED_START, DONT_ACKNOWLEDGE_BYTE);
        }
        else if (slaveAddress < MINIMUM_USABLE_7BIT_I2CADDRESS_08)
        {
        /* Add the 10-bit reserved conditions here */
        }
        else
        {
        /* Space for 10-bit addressing implementation */
        }
    }
}

/************************************************************************************************************
 *       Raw read from I2C bus.
 *       Returns status given by MW_I2C_MasterRead SVD function.
 *       Returns the data on success else undefined.
 ************************************************************************************************************/
void rawI2CRead(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{
    uint16_T index = 0;
    uint16_T slaveAddress = 0;
    uint32_T numBytesToRead = 0;
    uint8_T *data = NULL;
    MW_Handle_Type i2cHandle;

    /* Set status as no acknowledge by default */
    *status = MW_I2C_NO_ACKNOWLEDGE;

    /* Reads 1 byte bus number from the input buffer */
    uint32_T i2cBus = payloadBufferRx[index];
    index++;

    /* Reads 2 byte Slave address from the input buffer */
    slaveAddress = (uint8_T)payloadBufferRx[index] | ((uint8_T)payloadBufferRx[index+1]) << 8;
    index += 2;

    /* Reads 4 byte data length from the input buffer */
    numBytesToRead = (uint32_T)payloadBufferRx[index] | ((uint32_T)payloadBufferRx[index+1]) << 8 | ((uint32_T)payloadBufferRx[index+2]) << 16 | ((uint32_T)payloadBufferRx[index+3]) << 24;
    index += 4;

    /* Copies the pointer to the transmit buffer */
    data = (uint8_T*)&payloadBufferTx[*peripheralDataSizeResponse];

    /* Retrieve the I2C bus handle from the corresponding handle map */
    i2cHandle = getHandle(i2cBus, i2cMap);

    if ((MW_Handle_Type)NULL != i2cHandle)
    {
        /* Writes the data to the slave in 7-bit addressing format */
        if (slaveAddress >= MINIMUM_USABLE_7BIT_I2CADDRESS_08 && slaveAddress <= MAXIMUM_USABLE_7BIT_I2CADDRESS_119)
        {

            *status = MW_I2C_MasterRead(i2cHandle, slaveAddress, data, numBytesToRead, NO_REPEATED_START, DONT_ACKNOWLEDGE_BYTE);

#if DEBUG_FLAG == 1
            /* print debug message */
            debugPrint(MSG_RAW_I2CREAD, i2cBus, slaveAddress, numBytesToRead);
#endif

            if (MW_I2C_SUCCESS == *status)
            {
                /* Valid read with finite amount of data */
                *peripheralDataSizeResponse += numBytesToRead;
            }
            else
            {
                /* Invalid read with error condition */
                /* Error impl To be done after beta (g) */
                *peripheralDataSizeResponse += numBytesToRead;  /* Not right. But okay to move on */
            }
        }
        else if (slaveAddress < MINIMUM_USABLE_7BIT_I2CADDRESS_08)
        { /* Add the 10-bit reserved conditions here */
        /* Error impl To be done after beta (g) */
        *peripheralDataSizeResponse += numBytesToRead;  /* Not right. But okay to move on */
        }
        else
        {
        /* Space for 10-bit addressing implementation  */
        }
    }
}

/************************************************************************************************************
 *       Write into a register of an I2C slave.
 *       Returns status given by MW_I2C_MasterWrite SVD function.
 ************************************************************************************************************/
void registerI2CWrite(uint16_T* peripheralDataSizeResponse,uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{
    uint16_T index = 0;
    uint16_T slaveAddress = 0;
    uint32_T numBytesToWrite = 0;
    uint8_T *data = NULL;
    MW_Handle_Type i2cHandle;

    /* Set status as no acknowledge by default */
    *status = MW_I2C_NO_ACKNOWLEDGE;

#if DEBUG_FLAG == 1
    uint8_T tempVar = 0;
#endif
    /* Reads 1 byte bus number from the input buffer */
    uint32_T i2cBus = payloadBufferRx[index];
    index++;

    /* Reads 2 byte Slave address from the input buffer */
    slaveAddress = (uint8_T)payloadBufferRx[index] | ((uint8_T)payloadBufferRx[index+1]) << 8;
    index += 2;

    /* Reads 4 byte data length from the input buffer */
    numBytesToWrite = (uint32_T)payloadBufferRx[index] | ((uint32_T)payloadBufferRx[index+1]) << 8 | ((uint32_T)payloadBufferRx[index+2]) << 16 | ((uint32_T)payloadBufferRx[index+3]) << 24;
    index += 4;

    /* Copies the pointer to the data to be written (register address) */
    data = (uint8_T*)&payloadBufferRx[index];

    /* Retrieve the I2C bus handle from the corresponding handle map */
    i2cHandle = getHandle(i2cBus, i2cMap);

    if ((MW_Handle_Type)NULL != i2cHandle)
    {
        /* Writes the data to the slave in 7-bit addressing format */
        if (slaveAddress >= MINIMUM_USABLE_7BIT_I2CADDRESS_08 && slaveAddress <= MAXIMUM_USABLE_7BIT_I2CADDRESS_119)
        {

#if DEBUG_FLAG == 1
            /* print Debug message */
            debugPrint(MSG_REG_I2CWRITE,i2cBus, slaveAddress, *data);
            for (tempVar = 1;tempVar < numBytesToWrite; tempVar++)
            {
                debugPrint(MSG_REG_I2CWRITE1, *(data+tempVar));
            }
#endif
            *status = MW_I2C_MasterWrite(i2cHandle, slaveAddress, data, numBytesToWrite, NO_REPEATED_START, DONT_ACKNOWLEDGE_BYTE);
        }
        else if (slaveAddress < MINIMUM_USABLE_7BIT_I2CADDRESS_08)
        { /* Add the 10-bit reserved conditions here */
        }
        else
        {
        /* Space for 10-bit addressing implementation */
        }
    }
}

/************************************************************************************************************
 *       Read from a register of an I2C slave.
 *       Returns status given by MW_I2C_MasterWrite/MW_I2C_MasterRead SVD function.
*        Returns the data on success else undefined.
 ************************************************************************************************************/
void registerI2CRead(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{
    uint16_T index = 0;
    uint16_T slaveAddress = 0;
    uint16_T registerAddress = 0;
    uint32_T dataLength = 1;
    uint32_T numBytesToRead = 0;
    uint8_T *data = NULL;
    MW_Handle_Type i2cHandle;

    /* Set status as no acknowledge by default */
    *status = MW_I2C_NO_ACKNOWLEDGE;

    /* Reads 1 byte bus number from the input buffer */
    uint32_T i2cBus = payloadBufferRx[index];
    index++;

    /* Reads 2 byte Slave address from the input buffer */
    slaveAddress = (uint8_T)payloadBufferRx[index] | ((uint8_T)payloadBufferRx[index+1]) << 8;
    index += 2;

    /* Reads 1 byte Register Address from the input buffer */
    registerAddress = (uint8_T)payloadBufferRx[index];
    index++;

    /* Reads 4 byte data length from the input buffer */
    numBytesToRead = (uint32_T)payloadBufferRx[index] | ((uint32_T)payloadBufferRx[index+1]) << 8 | ((uint32_T)payloadBufferRx[index+2]) << 16 | ((uint32_T)payloadBufferRx[index+3]) << 24;
    index += 4;

    /* Copies the pointer to the transmit buffer */
    data = (uint8_T*)&payloadBufferTx[*peripheralDataSizeResponse];

        /* Retrieve the I2C bus handle from the corresponding handle map */
        i2cHandle = getHandle(i2cBus, i2cMap);

    if ((MW_Handle_Type)NULL != i2cHandle)
    {
        /* Writes the data to the slave in 7-bit addressing format */
        if (slaveAddress >= MINIMUM_USABLE_7BIT_I2CADDRESS_08 && slaveAddress <= MAXIMUM_USABLE_7BIT_I2CADDRESS_119)
        {

#if DEBUG_FLAG == 1
            /* print debug message */
            debugPrint(MSG_REG_I2CREAD1,i2cBus,slaveAddress,registerAddress);
#endif

            /* Selecting the register to be read */
            *status = MW_I2C_MasterWrite(i2cHandle, slaveAddress, (uint8_T *)&registerAddress, dataLength, REPEATED_START, DONT_ACKNOWLEDGE_BYTE);
            if (MW_I2C_SUCCESS == *status)
            {
                /* Successful selection of register to be read */

#if DEBUG_FLAG == 1
                /* print debug message */
                debugPrint(MSG_REG_I2CREAD2,i2cBus,slaveAddress, numBytesToRead);
#endif

                *status = MW_I2C_MasterRead(i2cHandle, slaveAddress, data, numBytesToRead, NO_REPEATED_START, DONT_ACKNOWLEDGE_BYTE);
                if (MW_I2C_SUCCESS == *status)
                {
                    /* Valid read with finite amount of data */
                    *peripheralDataSizeResponse += numBytesToRead;
                }
            }
        }
        else if (slaveAddress < MINIMUM_USABLE_7BIT_I2CADDRESS_08)
        { /* Add the 10-bit reserved conditions here */
        } else
        {
        /* Space for 10-bit addressing implementation */
        }
    }
}

/************************************************************************************************************************
 *       Sets the I2C Bus Frequency.
 *       Returns status given by MW_I2C_SetBusSpeed SVD function.
 ************************************************************************************************************************/
void setI2CFrequency(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{
    uint16_T index = 0;
    uint32_T i2cBusFrequency;
    MW_Handle_Type i2cHandle;

    /* Set status as no acknowledge by default */
    *status = MW_I2C_NO_ACKNOWLEDGE;

    /* Read 1 byte bus number from the input buffer */
    uint32_T i2cBus = payloadBufferRx[index++];

    /* Retrieve the I2C bus handle from the corresponding handle map */
    i2cHandle = getHandle(i2cBus, i2cMap);

    if ((MW_Handle_Type)NULL != i2cHandle)
    {
        /* Read 4 byte Frequency of bus from input buffer */
        i2cBusFrequency = (uint32_T)payloadBufferRx[index] | ((uint32_T)payloadBufferRx[index+1]) << 8 | ((uint32_T)payloadBufferRx[index+2]) << 16 | ((uint32_T)payloadBufferRx[index+3]) << 24;

#if DEBUG_FLAG == 1
        /* print debug message */
       debugPrint(MSG_SET_I2CFREQ,i2cBus,i2cBusFrequency);
#endif
        *status = MW_I2C_SetBusSpeed(i2cHandle, i2cBusFrequency);
    }
}

/***************************************************************************************************************************
 *      Opens the specified I2C bus and sets the specified frequency.
 *       Returns status given by MW_I2C_SetBusSpeed SVD function.
 ****************************************************************************************************************************/
void openI2CBus(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{
    uint16_T index = 0;
    uint32_T i2cBusFrequency;
    MW_Handle_Type i2cHandle;

    /* Set status as no acknowledge by default */
    *status = MW_I2C_NO_ACKNOWLEDGE;

    /* Read 1 byte bus number from the input buffer */
    uint32_T i2cBus = payloadBufferRx[index++];

    /* Read 4 byte Frequency of bus from input buffer */
    i2cBusFrequency = (uint32_T)payloadBufferRx[index] | ((uint32_T)payloadBufferRx[index+1]) << 8 | ((uint32_T)payloadBufferRx[index+2]) << 16 | ((uint32_T)payloadBufferRx[index+3]) << 24;

#if DEBUG_FLAG == 1
    /* print debug message */
    debugPrint(MSG_OPENI2CBUS1,i2cBus);
#endif

    /* Opens the bus */
    i2cHandle = MW_I2C_Open(i2cBus, MW_I2C_MASTER);
    if ((MW_Handle_Type)NULL != i2cHandle)
    {
        /* Stores the handle if open was successful */
        setHandle(i2cBus, i2cHandle, i2cMap);

#if DEBUG_FLAG == 1
        /* print debug message */
        debugPrint(MSG_OPENI2CBUS2,i2cBusFrequency);
#endif

        /* Sets the frequency of the I2C bus */
        *status = MW_I2C_SetBusSpeed(i2cHandle, i2cBusFrequency);

    }
}

/***************************************************************************************************************************
 *       Closes I2C Bus.
 *       Returns status given by MW_I2C_SetBusSpeed SVD function.
 ****************************************************************************************************************************/
void closeI2CBus(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{
    MW_Handle_Type i2cHandle;
    uint16_T index = 0;

    /* Set status as no acknowledge by default */
    *status = MW_I2C_NO_ACKNOWLEDGE;

    /* Read 1 byte bus number from the input buffer */
    uint32_T i2cBus = payloadBufferRx[index];

    /* Retrieve the I2C bus handle from the corresponding handle map */
    i2cHandle = getHandle(i2cBus, i2cMap);

#if DEBUG_FLAG == 1
    /* print debug message */
    debugPrint(MSG_CLOSEI2CBUS,i2cBus);
#endif

    if ((MW_Handle_Type)NULL != i2cHandle)
    {
        /* close the I2C bus */
        MW_I2C_Close(i2cHandle);
        setHandle(i2cBus, (MW_Handle_Type)NULL, i2cMap);
        /* Always passes because SVD doesn't return a status */
        *status = MW_I2C_SUCCESS;
    }
}

/***************************************************************************************************************************
 *      Required by Rasperry pi to mount the I2C driver module.
 ****************************************************************************************************************************/
void enableI2C(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{

}

/***************************************************************************************************************************
 *      Required by Rasperry pi to unmount the I2C driver module.
 ****************************************************************************************************************************/
void disableI2C(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{

}
#endif
