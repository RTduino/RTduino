/* Copyright 2015-2017 The MathWorks, Inc. */

#include "MW_PWM.h"

#if !(( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) ))
#include "Arduino.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define PWM_MODULES_MAX 70  /* For Mega and Due. Other boards have less number of Modules */

typedef uint8_T PeripheralHandleMapType;
extern PeripheralHandleMapType PWMPinHandleMap[];     /* The array is defined in ArduinoPinHandleMap.cpp */

/* For PWM SVd provides no function to get the handle */
MW_Handle_Type MW_PWM_GetHandle(uint32_T Pin);
void setPWMHandle(uint32_T Pin, MW_Handle_Type handle);

/* PWM Initialisation selected by the pinNumber (PWM Channel) */
MW_Handle_Type MW_PWM_Open(uint32_T pin, real_T frequency, real_T dutyCycle)
{
    #if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
    /* This will be run in Rapid Accelerator Mode */
    return NULL;
    #else
    pinMode(pin,OUTPUT);
    /* handle is stored as Pin+1 because, to reset the handle of a pin, 0 is passed as handle. Otherwise it will create confusion for Pin number 0. */
    setPWMHandle(pin, (MW_Handle_Type)(pin+1));
    return (MW_Handle_Type)(pin+1);
    #endif
}

/* Start PWM */
void MW_PWM_Start(MW_Handle_Type PWMPinHandle)
{
    (void)PWMPinHandle;
}

/* Set the duty cycle or pulse width for the PWM signal */
void MW_PWM_SetDutyCycle(MW_Handle_Type PWMPinHandle, real_T dutyCycle)
{
    #if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
    /* This will be run in Rapid Accelerator Mode */
    (void)PWMPinHandle;
    (void)dutyCycle;
    return;
    #else
    uint8_T pin;
    /* If the PWM handle is 0 then do nothing. */
    if((uint8_T)PWMPinHandle)
    {
    /* The handle of a Pin is stored as (Pin+1) */
    pin = (uint8_T)PWMPinHandle - 1;
    analogWrite(pin, dutyCycle);
    }
    #endif
}

/* Set the PWM signal frequency */
void MW_PWM_SetFrequency(MW_Handle_Type PWMPinHandle, real_T frequency)
{
    (void)PWMPinHandle;
    (void)frequency;
}

/* Disable notifications on the channel */
void MW_PWM_DisableNotification(MW_Handle_Type PWMPinHandle)
{
    (void)PWMPinHandle;
}

/* Enable notifications on the channel */
void MW_PWM_EnableNotification(MW_Handle_Type PWMPinHandle, MW_PWM_EdgeNotification_Type Notification)
{
    (void)PWMPinHandle;
    (void)Notification;
}

/* Set PWM output to idle state */
void MW_PWM_SetOutputToIdle(MW_Handle_Type PWMPinHandle)
{
    (void)PWMPinHandle;
}

/* Get the PWM output status */
boolean_T MW_PWM_GetOutputState(MW_Handle_Type PWMPinHandle)
{
    (void)PWMPinHandle;
    return 0;
}

/* Stop PWM */
void MW_PWM_Stop(MW_Handle_Type PWMPinHandle)
{
    (void)PWMPinHandle;
}

/* Close PWM */
void MW_PWM_Close(MW_Handle_Type PWMPinHandle)
{
    #if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
    return;
    #else
    /* If the PWM handle is 0 then do nothing. */
    if((uint8_T)PWMPinHandle)
    {
        uint8_T pin = 0;
        /* The handle of a Pin is stored as (Pin+1) */
        pin = (uint8_T)PWMPinHandle - 1;
        setPWMHandle(pin, (MW_Handle_Type)0);
    }
    (void)PWMPinHandle;
    #endif
}

MW_Handle_Type MW_PWM_GetHandle(uint32_T Pin)
{
    #if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
    return NULL;
    #else
    uint16_T ByteNo=0, handleValue=0;

        uint8_T BitPositionNo=0;

        ByteNo = Pin/8;
        BitPositionNo = Pin%8;

        handleValue = PWMPinHandleMap[ByteNo] & (PeripheralHandleMapType)(1<<BitPositionNo);

        if(handleValue)
        {
            /*
            Returns the value of the module if the pin is opened earlier
            */
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

void setPWMHandle(uint32_T module, MW_Handle_Type handle)
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
            PWMPinHandleMap[ByteNo] = PWMPinHandleMap[ByteNo] & tempVar;
        }
        else
        {
            /*set the handle*/
            PWMPinHandleMap[ByteNo] = PWMPinHandleMap[ByteNo] | tempVar;
        }
        #endif
}

#ifdef __cplusplus
}
#endif
