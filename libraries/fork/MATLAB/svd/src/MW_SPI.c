/* Copyright 2016-2017 The MathWorks, Inc. */

#include "MW_SPI.h"


/* Intialize SPI device */
MW_Handle_Type MW_SPI_Open(uint32_T SPIModule, uint32_T MosiPin, uint32_T MisoPin, uint32_T ClockPin, uint32_T SlaveSelectPin, uint8_T ActiveLowSSPin, uint8_T spi_device_type)
{
    return (MW_Handle_Type)NULL;
}

/* Set SPI Communication format for the processor */
    /* targetprecision: Bits/frame for processor */
    /* spi_mode: Clock polarity and clock phase */
    /* TargetFirstBitToTransfer: First bit to be transferred by processor */
MW_SPI_Status_Type MW_SPI_SetFormat(MW_Handle_Type SPIModuleHandle, uint8_T targetprecision, MW_SPI_Mode_type spi_mode, MW_SPI_FirstBitTransfer_Type TargetFirstBitToTransfer)
{
    return MW_SPI_SUCCESS;
}

/* Set SPI bus speed */
MW_SPI_Status_Type MW_SPI_SetBusSpeed(MW_Handle_Type SPIModuleHandle, uint32_T BusSpeedInHz)
{
    return MW_SPI_SUCCESS;
}

/* Configure SPI Slave select pin before starting the transmission if the slave select Pin is not custom */
MW_SPI_Status_Type MW_SPI_SetSlaveSelect(MW_Handle_Type SPIModuleHandle, uint32_T SlaveSelectPin, uint8_T ActiveLowSSPin)
{
    return MW_SPI_SUCCESS;
}

/* Initiate data communication from master to slave */
MW_SPI_Status_Type MW_SPI_MasterWriteRead_8bits(MW_Handle_Type SPIModuleHandle, const uint8_T * wrData, uint8_T * rdData, uint32_T datalength)
{
    return MW_SPI_SUCCESS;
}

/* Initiate data communication from master to slave in slave mode */
MW_SPI_Status_Type MW_SPI_SlaveWriteRead_8bits(MW_Handle_Type SPIModuleHandle, const uint8_T * wrData, uint8_T * rdData, uint32_T datalength)
{
    return MW_SPI_SUCCESS;
}

/* Get the status of SPI */
MW_SPI_Status_Type MW_SPI_GetStatus(MW_Handle_Type SPIModuleHandle)
{
    return MW_SPI_SUCCESS;
}

/* De-initialize or release the SPI */
void MW_SPI_Close(MW_Handle_Type SPIModuleHandle, uint32_T MosiPin, uint32_T MisoPin, uint32_T ClockPin, uint32_T SlaveSelectPin)
{

}
