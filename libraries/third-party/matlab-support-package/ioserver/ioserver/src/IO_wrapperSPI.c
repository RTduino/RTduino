/**
 * @file IO_wrapperSPI.c
 *
 * Contains definition of functions used for SPI communication
 *
 * @Copyright 2017-2019 The MathWorks, Inc.
 *
 */

#include "IO_wrapperSPI.h"
#if IO_STANDARD_SPI
static const uint16_T ZERO = 0;

#if DEBUG_FLAG == 1

#define OPENSPI "OpenSPI::MW_SPI_Open(spiBus %" PRIu32 ",MOSIPin %" PRIu32 ",MISOPin %" PRIu32 ",SCLKPin %" PRIu32 ",CSPin %" PRIu32 ");"
#define SETSPIFORMAT "SetSPIFormat::MW_SPI_SetFormat(spiBus %" PRIu32 ",targetPrecision %" PRIu32 ",spiMode %" PRIu32 ",bitOrder %s);"
#define SETSPIFREQ "SetBusSpeedSPI::MW_SPI_SetBusSpeed(spiBus %" PRIu32 ",frequency %" PRIu32 ");"
#define WRITEREADSPI1 "WriteReadSPI::MW_digitalIO_write(CSPin %" PRIu32 ",1-isCSPinActiveLow %" PRIu16 ");"
#define CLOSESPI "CloseSPI::MW_SPI_Close(spiBus %" PRIu32 ",MOSIPin %" PRIu32 ",MISOPin %" PRIu32 ",SCLKPin %" PRIu32 ",CSPin %" PRIu32 ");"
#define SPIFORLINUX "Enable the SPI peripheral for Linux based target"
#define UNMOUNT "Unmounting the SPI device peripheral"

const char MSG_OPENSPI[] PGMKEYWORD = OPENSPI;
const char MSG_SETSPIFORMAT[] PGMKEYWORD = SETSPIFORMAT;
const char MSG_SETSPIBUSFREQ[] PGMKEYWORD = SETSPIFREQ;
const char MSG_WRITEREADSPI1[] PGMKEYWORD = WRITEREADSPI1;
const char MSG_CLOSESPI[] PGMKEYWORD = CLOSESPI;
const char MSG_SPIFORLINUX[] PGMKEYWORD = SPIFORLINUX;
const char MSG_UNMOUNT[] PGMKEYWORD = UNMOUNT;

#endif

/* The IO_SPI_MODULES_MAX and IO_DIGITALIO_MODULES_MAX macros should be defined in the peripheralIncludes.h file */

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
PeripheralHandleMapType SPIMap[(IO_SPI_MODULES_MAX/8)+1] = {0};
extern PeripheralHandleMapType digitalIOMap[(IO_DIGITALIO_MODULES_MAX/8)+1];

#else
PeripheralHandleMapType SPIMap[IO_SPI_MODULES_MAX] = {NULL};
extern PeripheralHandleMapType digitalIOMap[IO_DIGITALIO_MODULES_MAX]; // The digitalIOMapMap is declared in the DigitalIO wrapper

#endif

/* Function to enable the SPI peripheral for Linux based target*/
void enableSPI(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{
    #if DEBUG_FLAG == 1
        debugPrint(MSG_SPIFORLINUX);
    #endif
    *status = MW_SPI_SUCCESS;
}

/* Function to set the properties of SPI bus and open the SPI bus*/
void openSPI(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{
    uint16_T index = ZERO;
    uint32_T MOSIPin, MISOPin, clockPin, slaveSelectPin, SPIBus, SPIBusFrequency;
    uint8_T SPITargetPrecision, isSPIDeviceMaster, isCSPinActiveLow;
    MW_SPI_Mode_type SPIMode;
    MW_SPI_FirstBitTransfer_Type bitOrder;
    MW_Handle_Type SPIHandle, digitalIOHandle;
    MW_SPI_Status_Type formatStatus, speedStatus;

    *status = MW_SPI_BUS_ERROR; // Set status as MW_SPI_BUS_ERROR by default
    /* Read SPI device properties from the data received from MATLAB */
    SPIBus = (uint32_T)payloadBufferRx[index++]; // SPI bus number
    MOSIPin = (uint32_T)payloadBufferRx[index++]; // Master Out Slave In pin number
    MISOPin = (uint32_T)payloadBufferRx[index++]; // Master In Slave Out pin number
    clockPin = (uint32_T)payloadBufferRx[index++]; //SPI clock pin number
    slaveSelectPin = (uint32_T)payloadBufferRx[index++]; // SPI slave select pin
    isCSPinActiveLow = payloadBufferRx[index++]; // Defines if the SPI device is an active low or active high enabled device
    isSPIDeviceMaster = payloadBufferRx[index++]; // Defines if the SPI device acts as a master or slave
    SPITargetPrecision = payloadBufferRx[index++]; // Defines the SPI device precision
    SPIMode = (MW_SPI_Mode_type)payloadBufferRx[index++]; // Defines the mode of operation of the SPI device
    bitOrder = (MW_SPI_FirstBitTransfer_Type)payloadBufferRx[index++]; // Defines the order of bit transfer (MSB first or LSB first)
    SPIBusFrequency = (uint32_T)payloadBufferRx[index] | ((uint32_T)payloadBufferRx[index+1]) << 8 | ((uint32_T)payloadBufferRx[index+2]) << 16 | ((uint32_T)payloadBufferRx[index+3]) << 24; // Defines the SPI bit rate
    index=index+4;

    digitalIOHandle = MW_digitalIO_open(slaveSelectPin,MW_DIGITALIO_OUTPUT); // Configure the slaveSelectPin for Output
    setHandle(slaveSelectPin, digitalIOHandle, digitalIOMap); // Save the DigitalIO handle in the digitalIOMap
    SPIHandle = MW_SPI_Open(SPIBus, MOSIPin, MISOPin, clockPin, slaveSelectPin, isCSPinActiveLow, isSPIDeviceMaster); // Open the SPI bus and save the SPI bus handle to SPIhandle
    MW_digitalIO_write(digitalIOHandle,isCSPinActiveLow); //Disable the SPI device
    #if DEBUG_FLAG == 1
        /* print debug message */
        debugPrint(MSG_OPENSPI,SPIBus, MOSIPin, MISOPin, clockPin, slaveSelectPin);
    #endif

    /* If both the SPI and DigitalIO handles are valid, set the SPI bus format and speed followed by opening the SPI bus */
    if ((MW_Handle_Type)NULL != SPIHandle && (MW_Handle_Type)NULL != digitalIOHandle) {
        setHandle(SPIBus, SPIHandle, SPIMap); // Save the handle into SPIMap
        formatStatus = MW_SPI_SetFormat(SPIHandle, SPITargetPrecision, SPIMode, bitOrder); // Set SPI Bus format and return the status
        if(MW_SPI_SUCCESS == formatStatus){
        speedStatus = MW_SPI_SetBusSpeed(SPIHandle, SPIBusFrequency); // Set the SPI bus speed and return the status
            *status = speedStatus;
        }
        else {
            *status = formatStatus;
        }
    }
}

/* Function to set the SPI Mode and SPI bit order*/
void setFormatSPI(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{
    uint16_T index = ZERO;
    MW_Handle_Type SPIHandle;
    uint32_T SPIBus;
    uint8_T SPITargetPrecision;
    MW_SPI_Mode_type SPIMode;
    MW_SPI_FirstBitTransfer_Type bitOrder;

    *status = MW_SPI_BUS_ERROR; // Set status as MW_SPI_BUS_ERROR by default
    SPIBus = (uint32_T)payloadBufferRx[index++]; // SPI bus number
    SPITargetPrecision = (uint32_T)payloadBufferRx[index++]; // Defines the SPI device precision
    SPIMode = (uint32_T)payloadBufferRx[index++]; // Defines the mode of operation of the SPI device
    bitOrder = (uint32_T)payloadBufferRx[index++]; // Defines the order of bit transfer (MSB first or LSB first)
    SPIHandle = getHandle(SPIBus, SPIMap); // Get the SPI peripheral(bus) handle from SPIMap
    #if DEBUG_FLAG == 1
    if (1 == bitOrder)
    {
        /* print debug message */
        debugPrint(MSG_SETSPIFORMAT, SPIBus, SPITargetPrecision, (uint32_T)SPIMode, "lsbfirst");
    }
    else
    {
        /* print debug message */
        debugPrint(MSG_SETSPIFORMAT,(uint32_T)SPIBus,(uint32_T)SPITargetPrecision, (uint32_T)SPIMode, "msbfirst");
    }
    #endif
    if ((MW_Handle_Type)NULL != SPIHandle) {
        *status = MW_SPI_SetFormat(SPIHandle, SPITargetPrecision, SPIMode, bitOrder); // Set SPI Bus format and return the status
        }
}

/* Function to set the SPI bus frequency*/
void setBusSpeedSPI(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{
    uint16_T index = ZERO;
    MW_Handle_Type SPIHandle;
    uint32_T SPIBus,SPIBusFrequency;

    *status = MW_SPI_BUS_ERROR; // Set status as MW_SPI_BUS_ERROR by default
    SPIBus = (uint32_T)payloadBufferRx[index++]; // SPI bus number
    SPIBusFrequency = (uint32_T)payloadBufferRx[index] | ((uint32_T)payloadBufferRx[index+1]) << 8 | ((uint32_T)payloadBufferRx[index+2]) << 16 | ((uint32_T)payloadBufferRx[index+3]) << 24; // Defines the SPI bit rate
    index = index+4;
    SPIHandle = getHandle(SPIBus, SPIMap); // Get the SPI peripheral(bus) handle from SPIMap

    #if DEBUG_FLAG == 1
        /* print debug message */
        debugPrint(MSG_SETSPIBUSFREQ,SPIBus,SPIBusFrequency);
    #endif

    if ((MW_Handle_Type)NULL != SPIHandle) {
        *status = MW_SPI_SetBusSpeed(SPIHandle,SPIBusFrequency); // Set the SPI bus frequency and return the status
        }
}

/* Function to write into and read from an SPI device*/
void writeReadSPI(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{
    uint16_T index = ZERO;
    uint32_T SPIBus,dataLength,slaveSelectPin,ctr;
    const uint8_T *wrData;
    uint8_T *rdData;
    uint8_T isCSPinActiveLow;
    MW_Handle_Type SPIHandle,digitalIOHandle;
    #if DEBUG_FLAG == 1
        uint8_T tmp_var = 0;
    #endif
    *status = MW_SPI_BUS_ERROR; // Set status as MW_SPI_BUS_ERROR by default
    /*  Extract the data sent from MATLAB contained in the Receive buffer */
    SPIBus = (uint32_T)payloadBufferRx[index++]; // SPI bus number
    slaveSelectPin = (uint32_T)payloadBufferRx[index++]; // slave select pin number
    isCSPinActiveLow = (uint8_T)payloadBufferRx[index++]; // Defines if the SPI device is an active low or active high enabled device
    dataLength = (uint32_T)payloadBufferRx[index] | ((uint32_T)payloadBufferRx[index+1]) << 8 | ((uint32_T)payloadBufferRx[index+2]) << 16 | ((uint32_T)payloadBufferRx[index+3]) << 24; // Data length in bytes
    index = index+4;
    wrData = (uint8_T *)&payloadBufferRx[index++]; // Pointer to the first location of the data to be written
    rdData = (uint8_T*) malloc(dataLength); // Array to store the data read from SPI device

    /* Retrieve the SPI bus handle and the DigitalIO handle corresponding to slave select pin from the respective handle map*/
    digitalIOHandle = getHandle(slaveSelectPin, digitalIOMap);
    SPIHandle = getHandle(SPIBus, SPIMap);

    /* If both the DigitalIO and SPI handle are valid, then write data to SPI */
    if ((MW_Handle_Type)NULL != SPIHandle && (MW_Handle_Type)NULL != digitalIOHandle) {
        MW_digitalIO_write(digitalIOHandle,1-isCSPinActiveLow); // Enable the SPI device before writing to device

        #if DEBUG_FLAG == 1
            /* print debug message */
            debugPrint(MSG_WRITEREADSPI1,slaveSelectPin,((uint8_T)1-(uint8_T)isCSPinActiveLow));
        #endif
        #ifdef LTC_BAREMETAL_HARDWARE
            //g2060070 Arduino needs to enable slaveSelectPin after calling beginTransacation with settings, so using SPIModuleNumber placeholder to send slaveSelectSPin number to SVD Layer as Arduino doesn't have multiple SPI buses
            *status = MW_SPI_MasterWriteRead_8bits(digitalIOHandle,wrData,rdData,dataLength); // Write the data to SPI device and return status
        #else
            *status = MW_SPI_MasterWriteRead_8bits(SPIHandle,wrData,rdData,dataLength);
        #endif
        MW_digitalIO_write(digitalIOHandle,isCSPinActiveLow); // Disable the SPI device after writing to device

        if (MW_SPI_SUCCESS == *status) {
            for (ctr=ZERO; ctr<dataLength; ctr++) {
                payloadBufferTx[(*peripheralDataSizeResponse)++] = rdData[ctr]; // Store the data that is read from SPI device in the transmit buffer
            }
        }
    }
    /* Deallocate the memory that was reserved for rdData */
    if (rdData != NULL) {
        free(rdData);
        rdData = NULL;
    }
}

/* Function to close the SPI bus */
void closeSPI(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{
    uint16_T index = ZERO;
    uint32_T MOSIPin, MISOPin, clockPin, slaveSelectPin, SPIBus;
    MW_Handle_Type SPIHandle,digitalIOHandle;

    *status = MW_SPI_BUS_ERROR; // Set status as MW_SPI_BUS_ERROR by default
    /* Read the SPI bus properties from the Receive buffer */
    SPIBus = (uint32_T)payloadBufferRx[index++];
    MOSIPin = (uint32_T)payloadBufferRx[index++];
    MISOPin = (uint32_T)payloadBufferRx[index++];
    clockPin = (uint32_T)payloadBufferRx[index++];
    slaveSelectPin = (uint32_T)payloadBufferRx[index++];

    /* Retrieve the SPI bus handle and the DigitalIO handle corresponding to slave select pin from the respective handle map*/
    SPIHandle = getHandle(SPIBus, SPIMap);
    digitalIOHandle = getHandle(slaveSelectPin, digitalIOMap);

    /* If both the above handles are vaild, then close the SPI bus and slaveSelectPin and return true. If either of the above handles are invalid, return false*/
    if ((MW_Handle_Type)NULL != SPIHandle && (MW_Handle_Type)NULL != digitalIOHandle) {

        MW_SPI_Close(SPIHandle, MOSIPin, MISOPin, clockPin, slaveSelectPin); // Close SPI bus

        #if DEBUG_FLAG == 1
            /* print debug message */
            debugPrint(MSG_CLOSESPI, SPIBus, MOSIPin, MISOPin, clockPin, slaveSelectPin);
        #endif

        MW_digitalIO_close(digitalIOHandle); // Close the Slave Select pin
        setHandle(SPIBus,(MW_Handle_Type)NULL,SPIMap); // Make the corresponding SPI handle to NULL
        setHandle(slaveSelectPin,(MW_Handle_Type)NULL,digitalIOMap); // Make the corresponding DigitalIO handle to NULL
        *status = MW_SPI_SUCCESS; // Always passes because SVD doesn't return a status
    }
}

/* Function to disable or unmount the SPI device peripheral*/
void disableSPI(uint16_T* peripheralDataSizeResponse, uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint8_T* status)
{
    #if DEBUG_FLAG == 1
        /* print debug message */
        debugPrint(MSG_UNMOUNT);
    #endif
    *status = MW_SPI_SUCCESS;
}
#endif
