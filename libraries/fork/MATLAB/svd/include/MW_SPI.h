/* Copyright 2016-2017 The MathWorks, Inc. */

#ifndef __MW_SPI_H__
#define __MW_SPI_H__

#include "MW_digitalIO.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _MW_SPI_Status_Type {
    MW_SPI_SUCCESS           = 0x00,
    MW_SPI_BUSY              = 0x01,
    MW_SPI_BUS_ERROR         = 0x02,
    MW_SPI_BUS_NOT_IN_USE    = 0x04,
    MW_SPI_BUS_NOT_AVAILABLE = 0x08
} MW_SPI_Status_Type;

typedef enum _MW_SPI_FirstBitTransfer_Type {
    MW_SPI_LEAST_SIGNIFICANT_BIT_FIRST = 0,
    MW_SPI_MOST_SIGNIFICANT_BIT_FIRST
} MW_SPI_FirstBitTransfer_Type;

typedef enum _MW_SPI_Mode_type {
    MW_SPI_MODE_0 = 0,
    MW_SPI_MODE_1,
    MW_SPI_MODE_2,
    MW_SPI_MODE_3
} MW_SPI_Mode_type;

/* Intialize SPI device */
MW_Handle_Type MW_SPI_Open(uint32_T SPIModule, uint32_T MosiPin, uint32_T MisoPin, uint32_T ClockPin, uint32_T SlaveSelectPin, uint8_T ActiveLowSSPin, uint8_T IsSPIDeviceMaster);
/* Set SPI Communication format for the processor */
    /* TargetPrecision: Bits/frame for processor */
    /* SPIMode: Clock polarity and clock phase */
    /* TargetFirstBitToTransfer: First bit to be transferred by processor */
MW_SPI_Status_Type MW_SPI_SetFormat(MW_Handle_Type SPIModuleHandle, uint8_T TargetPrecision, MW_SPI_Mode_type SPIMode, MW_SPI_FirstBitTransfer_Type TargetFirstBitToTransfer);
/* Configure SPI Slave select pin before starting the transmission if the slave select Pin is not custom */
MW_SPI_Status_Type MW_SPI_SetSlaveSelect(MW_Handle_Type SPIModuleHandle, uint32_T SlaveSelectPin, uint8_T ActiveLowSSPin);
/* Set SPI bus speed */
MW_SPI_Status_Type MW_SPI_SetBusSpeed(MW_Handle_Type SPIModuleHandle, uint32_T BusSpeedInHz);
/* Initiate data communication from master to slave */
MW_SPI_Status_Type MW_SPI_MasterWriteRead_8bits(MW_Handle_Type SPIModuleHandle, const uint8_T * WriteDataPtr, uint8_T * ReadDataPtr, uint32_T DataLength);
/* Initiate data communication from master to slave in slave mode */
MW_SPI_Status_Type MW_SPI_SlaveWriteRead_8bits(MW_Handle_Type SPIModuleHandle, const uint8_T * WriteDataPtr, uint8_T * ReadDataPtr, uint32_T DataLength);
/* Get the status of SPI */
MW_SPI_Status_Type MW_SPI_GetStatus(MW_Handle_Type SPIModuleHandle);
/* De-initialize or release the SPI */
void MW_SPI_Close(MW_Handle_Type SPIModuleHandle, uint32_T MosiPin, uint32_T MisoPin, uint32_T ClockPin, uint32_T SlaveSelectPin);

#ifdef __cplusplus
}
#endif

#endif /* __MW_SPI_H__ */
