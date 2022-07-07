/* Copyright 2015-2018 The MathWorks, Inc. */
#include "MW_I2C.h"
#if !(( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) ))
    #ifdef __MW_TARGET_USE_HARDWARE_RESOURCES_H__
            #include "MW_target_hardware_resources.h"
    #endif

    #include "Arduino.h"
    #include <Wire.h>
    #include "io_wrappers.h"
    #include <inttypes.h>
    #include "rtwtypes.h"

    /* Structure to hold the I2C module details*/
    typedef struct {
          uint8_T isInit;
          uint8_T I2C_module;
          TwoWire *I2Cobj;
        } mw_I2C_dev_h;

    /* WIRE_INTERFACES_COUNT is defined in variants.h (Arduino IDE) only for ARM processors*/
    #ifndef WIRE_INTERFACES_COUNT
        #define WIRE_INTERFACES_COUNT  1
    #endif

    static mw_I2C_dev_h mw_I2C_dev[WIRE_INTERFACES_COUNT] = {
        {0,0,&Wire}
        #if WIRE_INTERFACES_COUNT > 1
        ,{0,1,&Wire1} // For Due
        #endif
    };
#endif

#ifdef __cplusplus
    extern "C" {
#endif

/* Initialize a I2C */
MW_Handle_Type MW_I2C_Open(uint32_T I2CModule, MW_I2C_Mode_Type i2c_mode)
{
    #if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
        /* This will be run in Rapid Accelerator Mode */
        return NULL;
    #else
        /*Initialize the I2C module once*/
        if ((I2CModule < WIRE_INTERFACES_COUNT) && (0 == mw_I2C_dev[I2CModule].isInit))
        {
            (mw_I2C_dev[I2CModule].I2Cobj)->begin();
            mw_I2C_dev[I2CModule].isInit = 1;
        }
        return (MW_Handle_Type)&mw_I2C_dev[I2CModule];
    #endif
}

/* Set the I2C bus speed in Master Mode */
MW_I2C_Status_Type MW_I2C_SetBusSpeed(MW_Handle_Type I2CModuleHandle, uint32_T BusSpeed)
{
    #if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
        /* This will be run in Rapid Accelerator Mode */
        return MW_I2C_SUCCESS;
    #else
        if (I2CModuleHandle != NULL)
        {
             /* Override the BusSpeed from configset using macros
             * If macros not defined then use default 100,000 Hz init
             * value for BusSpeed
             */
            #ifdef MW_I2C_I2C0BUSSPEEDHZ
                if (((mw_I2C_dev_h*)I2CModuleHandle)->I2C_module == 0)
                    BusSpeed = MW_I2C_I2C0BUSSPEEDHZ; // in Hz
            #endif

            #ifdef MW_I2C_I2C1BUSSPEEDHZ
                 if (((mw_I2C_dev_h*)I2CModuleHandle)->I2C_module == 1)
                    BusSpeed = MW_I2C_I2C1BUSSPEEDHZ; // in Hz
            #endif

            ((mw_I2C_dev_h*)I2CModuleHandle)->I2Cobj->setClock(BusSpeed);
            return MW_I2C_SUCCESS;
        }
        else
            return MW_I2C_BUS_ERROR; // No valid handle to set bus speed
    #endif
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
    #if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
        /* This will be run in Rapid Accelerator Mode */
        return MW_I2C_SUCCESS;
    #else
        if (I2CModuleHandle != NULL)
        {
            /* Reads quantity number of bytes from I2C bus.
             * Checks for Repeated Start case and makes the flag false.
             * Enables interrupts but does not issue another START.
             * Waits for read operation to complete in blocking mode.
             * Takes care of sending ACKs for all but last byte and NACK for last byte received.
             * Handles releasing the I2C bus when in bad state.
             */
            ((mw_I2C_dev_h*)I2CModuleHandle)->I2Cobj->requestFrom((uint8_T)SlaveAddress,(size_t)DataLength);
            uint32_T i = 0;
            /*Check if Receive buffer has any data from I2C slave*/
            if (((mw_I2C_dev_h*)I2CModuleHandle)->I2Cobj->available())
            {
                while ((((mw_I2C_dev_h*)I2CModuleHandle)->I2Cobj->available()) && (i < DataLength))
                {
                    int temp_data  = ((mw_I2C_dev_h*)I2CModuleHandle)->I2Cobj->read();
                    if (temp_data == -1)
                    {
                        memset(data, 0, sizeof(DataLength));
                        return MW_I2C_BUS_ERROR; /*No data read*/
                    }
                    else
                        data[i++] = (uint8_T)temp_data ;
                } //while
            }
            else
            {
                 /*When the I2C slave does not respond or gets disconnected*/
                 memset(data, 0, sizeof(DataLength));
                 return MW_I2C_BUS_ERROR;
            }
            return MW_I2C_SUCCESS; /*On successful read*/
        }
        else
            return MW_I2C_BUS_ERROR;  /*If I2C dev handle is NULL*/
    #endif
}

/* Send the data from master to a specified slave */
MW_I2C_Status_Type MW_I2C_MasterWrite(MW_Handle_Type I2CModuleHandle, uint16_T SlaveAddress, uint8_T * data, uint32_T DataLength, uint8_T RepeatedStart, uint8_T SendNoAck)
{
    #if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
        /* This will be run in Rapid Accelerator Mode */
        return MW_I2C_SUCCESS;
    #else
        if (I2CModuleHandle!= NULL)
        {
            uint8_T res = 0;
            /* Saves SlaveAddress into a class variable*/
            ((mw_I2C_dev_h*)I2CModuleHandle)->I2Cobj->beginTransmission((uint8_T)SlaveAddress);
            /* Copies data to a software transmit buffer by looping DataLength times*/
            ((mw_I2C_dev_h*)I2CModuleHandle)->I2Cobj->write(data, (size_t)DataLength);
            /* Sends START condition, SlaveAddress + Write bit,
             * Data over I2C bus and ends by STOP condition.
             */

            if (RepeatedStart)
            {
                // Repeated Start for Register Read
                res = ((mw_I2C_dev_h*)I2CModuleHandle)->I2Cobj->endTransmission(false);
            }
            else
            {
                res = ((mw_I2C_dev_h*)I2CModuleHandle)->I2Cobj->endTransmission(true);
            }
            /* List of errors :
             * 0:success
             * 1:data too long to fit in transmit buffer
             * 2:received NACK on transmit of address
             * 3:received NACK on transmit of data
             * 4:other error
             */
            if (res == 0)
                return MW_I2C_SUCCESS;
            else if ((res == 2) || (res == 3))
                return MW_I2C_NO_ACKNOWLEDGE;
            else
                return MW_I2C_BUS_ERROR;
        }
        else
        {
            return MW_I2C_BUS_ERROR;
        }
    #endif
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
    #if !( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
        if (I2CModuleHandle!= NULL){
            ((mw_I2C_dev_h*)I2CModuleHandle)->I2Cobj->end();
        }
    #endif
}

#ifdef __cplusplus
}
#endif
