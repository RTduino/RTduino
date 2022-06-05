/* Copyright 2017-2019 The MathWorks, Inc. */

#include "MW_AnalogIn.h"

#if !(( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) ))
#include "Arduino.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define ANALOG_MODULES_MAX 70  /* For Mega and Due. Other boards have less number of Modules */

typedef uint8_T PeripheralHandleMapType;
extern PeripheralHandleMapType analogPinHandleMap[];    /* The array is defined in ArduinoPinHandleMap.cpp */

void setHandle(uint32_T Pin, MW_Handle_Type handle);

/* Create AnalogIn group with Channels and Conversion time */
MW_Handle_Type MW_AnalogInSingle_Open(uint32_T Pin)
{
    #if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
    /* This will be run in Rapid Accelerator Mode */
    return NULL;
    #else
    if (Pin < A0)
    {
        pinMode(Pin+A0, INPUT);
    }
    else{
        pinMode(Pin, INPUT);
    }
    /* handle is stored as Pin+1 because, to reset the handle of a pin, 0 is passed as handle. Otherwise it will create confusion for Pin number 0. */
    setHandle(Pin, (MW_Handle_Type)(Pin+1));
    return (MW_Handle_Type)(Pin+1);
    #endif

}

/* Select trigger source for AnalogIn group to start conversion */
void MW_AnalogIn_SetTriggerSource(MW_Handle_Type AnalogInHandle, MW_AnalogIn_TriggerSource_Type TriggerType, uint32_T TriggerValue)
{
    (void)AnalogInHandle;
    (void)TriggerType;
    (void)TriggerValue;
}

/* Enable Conversion complete notification */
void MW_AnalogIn_EnableNotification(MW_Handle_Type AnalogInHandle)
{
    (void)AnalogInHandle;
}

/* Disable notifications */
void MW_AnalogIn_DisableNotification(MW_Handle_Type AnalogInHandle)
{
    (void)AnalogInHandle;
}

/* Enable continuous conversion */
void MW_AnalogIn_EnableContConversion(MW_Handle_Type AnalogInHandle)
{
    (void)AnalogInHandle;
}

/* Set channel conversion priority */
void MW_AnalogIn_SetChannelConvRank(MW_Handle_Type AnalogInHandle, uint32_T Channel, uint32_T Rank)
{
    (void)AnalogInHandle;
    (void)Channel;
    (void)Rank;
}

/* Get status of AnalogIn conversion group */
MW_AnalogIn_Status_Type MW_AnalogIn_GetStatus(MW_Handle_Type AnalogInHandle)
{
    return MW_ANALOGIN_CONVERSION_COMPLETE;
}

/* Read channel conversion result */
void MW_AnalogInSingle_ReadResult(MW_Handle_Type AnalogInHandle, void * Result_ptr, MW_AnalogIn_ResultDataType_Type ResultDataType)
{
    #if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
    /* This will be run in Rapid Accelerator Mode */
    return;
    #else
    uint8_T pin;
    /* If a non zero handle is passed then read the analog input. Otherwise return 0. This
     is useful for MATLAB codegen is the user tries to read without configuring. In Simulink
     this situation should never arise*/
    if((uint8_T)AnalogInHandle)
    {
    /* The handle of a Pin is stored as (Pin+1) */
    pin = (uint8_T)AnalogInHandle - 1;
    /*Use 12-bit read resolution for Due, MKR1000*/
    #if defined(ARDUINO_SAM_DUE) || defined(ARDUINO_SAMD_MKR1000) || defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_MKRZERO)
      analogReadResolution(12);
    #endif
    int sensorValue = analogRead(pin);
    *(uint16_T *)Result_ptr = sensorValue;
    }
    else{
        *(uint16_T *)Result_ptr = 0;
    }
    #endif
}

/* Start conversion */
void MW_AnalogIn_Start(MW_Handle_Type AnalogInHandle)
{
   (void)AnalogInHandle;
}

/* Stop conversion */
void MW_AnalogIn_Stop(MW_Handle_Type AnalogInHandle)
{
    (void)AnalogInHandle;
}

/* De-initialise */
void MW_AnalogIn_Close(MW_Handle_Type AnalogInHandle)
{
    #if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
    return;
    #else
    /* If the analog handle is 0 then do nothing. */
    if((uint8_T)AnalogInHandle)
    {
        uint8_T pin = 0;
        /* The handle of a Pin is stored as (Pin+1) */
        pin = (uint8_T)AnalogInHandle - 1;
        /* Reset the handle for that particular pin */
        setHandle(pin, (MW_Handle_Type)0);
    }
    (void)AnalogInHandle;
    #endif
}

MW_Handle_Type MW_AnalogIn_GetHandle(uint32_T Pin)
{
    #if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
    return NULL;
    #else
    uint16_T ByteNo=0, handleValue=0;

        uint8_T BitPositionNo=0;

        ByteNo = Pin/8;
        BitPositionNo = Pin%8;

        handleValue = analogPinHandleMap[ByteNo] & (PeripheralHandleMapType)(1<<BitPositionNo);

        if(handleValue)
        {
            /*
            Returns the value of the module if the pin is opened earlier
            */
            /* What will happen if Pin value is 0 itself? */
            handleValue = Pin+1;
            /*
            This allows to differentiate between pin zero and a Null condition

            Another option - return true, and then remove the decrement in the SVD of all functions in Arduino
            */
        }
        else
        {
            handleValue = 0;
        }
        return (MW_Handle_Type)handleValue;
        #endif
}

void setHandle(uint32_T module, MW_Handle_Type handle)
{
    #if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
    return;
    #else
    /*
    Assumes pin number from count zero
    */
        uint16_T ByteNo=0;
        uint8_T BitPositionNo=0;
        PeripheralHandleMapType tempVar=0;
        /*
        Integer division rounds off to value providing the abs byte no
        */
        ByteNo = module/8;
        BitPositionNo = module%8;

        tempVar = (1<<BitPositionNo);
        /*
        The assumption is module number is the handle,
        but when handle is Null, reset the handle
        */
        if(!handle)
        {
            /*reset the handle*/
            tempVar = ~tempVar;
            analogPinHandleMap[ByteNo] = analogPinHandleMap[ByteNo] & tempVar;
        }
        else
        {
            /*set the handle*/
            analogPinHandleMap[ByteNo] = analogPinHandleMap[ByteNo] | tempVar;
        }
        #endif
}

#ifdef __cplusplus
}
#endif
