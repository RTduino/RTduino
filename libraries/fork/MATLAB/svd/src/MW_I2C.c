/* Copyright 2015-2017 The MathWorks, Inc. */
#include "MW_I2C.h"

/* Initialize a I2C */
MW_Handle_Type MW_I2C_Open(uint32_T I2CModule, MW_I2C_Mode_Type i2c_mode)
{
    return (MW_Handle_Type)NULL;
}

/* Set the I2C bus speed in Master Mode */
MW_I2C_Status_Type MW_I2C_SetBusSpeed(MW_Handle_Type I2CModuleHandle, uint32_T BusSpeed)
{
    return MW_I2C_SUCCESS;
}

/* Set the slave address (used only by slave) */
MW_I2C_Status_Type MW_I2C_SetSlaveAddress(MW_Handle_Type I2CModuleHandle, uint32_T SlaveAddress)
{
    return MW_I2C_SUCCESS;
}

/* Initiate I2C communication, send a start signal on I2C bus. */
MW_I2C_Status_Type MW_I2C_Start(MW_Handle_Type I2CModuleHandle)
{
    return MW_I2C_SUCCESS;
}

/* Receive the data on Master device from a specified slave */
MW_I2C_Status_Type MW_I2C_MasterRead(MW_Handle_Type I2CModuleHandle, uint16_T SlaveAddress, uint8_T * data, uint32_T DataLength, uint8_T RepeatedStart, uint8_T SendNoAck)
{
    return MW_I2C_SUCCESS;
}

/* Send the data from master to a specified slave */
MW_I2C_Status_Type MW_I2C_MasterWrite(MW_Handle_Type I2CModuleHandle, uint16_T SlaveAddress, uint8_T * data, uint32_T DataLength, uint8_T RepeatedStart, uint8_T SendNoAck)
{
    return MW_I2C_SUCCESS;
}

/* Read data on the slave device from a Master */
MW_I2C_Status_Type MW_I2C_SlaveRead(MW_Handle_Type I2CModuleHandle, uint8_T * data, uint32_T DataLength, uint8_T SendNoAck)
{
    return MW_I2C_SUCCESS;
}

/* Send the data to a master from the slave */
MW_I2C_Status_Type MW_I2C_SlaveWrite(MW_Handle_Type I2CModuleHandle, uint8_T * data, uint32_T DataLength, uint8_T SendNoAck)
{
    return MW_I2C_SUCCESS;
}

/* Get the status of I2C device */
MW_I2C_Status_Type MW_I2C_GetStatus(MW_Handle_Type I2CModuleHandle)
{
    return MW_I2C_SUCCESS;
}

/* Terminate the I2C communication */
MW_I2C_Status_Type MW_I2C_Stop(MW_Handle_Type I2CModuleHandle)
{
    return MW_I2C_SUCCESS;
}

/* Release I2C module */
void MW_I2C_Close(MW_Handle_Type I2CModuleHandle)
{
}

#ifdef __cplusplus
}
#endif
