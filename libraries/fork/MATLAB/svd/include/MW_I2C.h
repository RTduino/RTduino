/* Copyright 2015-2017 The MathWorks, Inc. */
#ifndef __MW_I2C_H__
#define __MW_I2C_H__
#include "MW_SVD.h"

typedef enum _MW_I2C_Status_Type {
    MW_I2C_SUCCESS          = 0x00,
    MW_I2C_BUSY             = 0x01,
    MW_I2C_ARBITRATION_LOST = 0x02,
    MW_I2C_NO_ACKNOWLEDGE   = 0x04,
    MW_I2C_BUS_ERROR        = 0x08,
    MW_I2C_BUS_NOT_IN_USE   = 0x10
} MW_I2C_Status_Type;

typedef enum _MW_I2C_Mode_Type {
    MW_I2C_MASTER = 0,
    MW_I2C_Slave
} MW_I2C_Mode_Type;


#ifdef __cplusplus
extern "C" {
#endif

/* Initialize a I2C */
MW_Handle_Type MW_I2C_Open(uint32_T I2CModule, MW_I2C_Mode_Type i2c_mode);

/* Set the I2C bus speed in Master Mode */
MW_I2C_Status_Type MW_I2C_SetBusSpeed(MW_Handle_Type I2CModuleHandle, uint32_T BusSpeed);

/* Set the slave address (used only by slave) */
MW_I2C_Status_Type MW_I2C_SetSlaveAddress(MW_Handle_Type I2CModuleHandle, uint32_T SlaveAddress);

/* Receive the data on Master device from a specified slave */
MW_I2C_Status_Type MW_I2C_MasterRead(MW_Handle_Type I2CModuleHandle, uint16_T SlaveAddress, uint8_T * data, uint32_T DataLength, uint8_T RepeatedStart, uint8_T SendNoAck);

/* Send the data from master to a specified slave */
MW_I2C_Status_Type MW_I2C_MasterWrite(MW_Handle_Type I2CModuleHandle, uint16_T SlaveAddress, uint8_T * data, uint32_T DataLength, uint8_T RepeatedStart, uint8_T SendNoAck);

/* Read data on the slave device from a Master */
MW_I2C_Status_Type MW_I2C_SlaveRead(MW_Handle_Type I2CModuleHandle, uint8_T * data, uint32_T DataLength, uint8_T SendNoAck);

/* Send the data to a master from the slave */
MW_I2C_Status_Type MW_I2C_SlaveWrite(MW_Handle_Type I2CModuleHandle, uint8_T * data, uint32_T DataLength, uint8_T SendNoAck);

/* Get the status of I2C device */
MW_I2C_Status_Type MW_I2C_GetStatus(MW_Handle_Type I2CModuleHandle);

/* Release I2C module */
void MW_I2C_Close(MW_Handle_Type I2CModuleHandle);

#ifdef __cplusplus
}
#endif

#endif /* __MW_I2C_H__ */
