/**
 * @file IO_wrapperPWM.c
 *
 * Wraps around PWM SVD C layer to provide features to access PWM pins through IO Server
 *
 * @Copyright 2017-2018 The MathWorks, Inc.
 *
 *
 */
#include "IO_wrapperPWM.h"

#if IO_STANDARD_PWM
#if ESB_BETA1_IOSERVER
/* Considered from stm32f7xx_hal_tim.h */
#define TIM_COUNTERMODE_UP                 ((uint32_T)0x0000U)
void MW_PWM_SetCounterMode(MW_Handle_Type, uint32_T);
#endif

#if DEBUG_FLAG == 1
#define STARTPWM "StartPWM::MW_PWM_Start(pin %" PRIu32 ");"
#if (1 == FLOAT_ENABLED)
#if ESB_BETA2_IOSERVER
#define CONFPIN "ConfigurePWMPin::MW_PWM_Open(pin %" PRIu32 ",counter %s ,frequency %f ,dutyCycle %f%%);"
#else
#define CONFPIN "ConfigurePWMPin::MW_PWM_Open(pin %" PRIu32 ",frequency %f ,dutyCycle %f%%);"
#endif
#define SETFREQUENCY "SetFrequency::MW_PWM_SetFrequency(pin %" PRIu32 ",frequency %f);"
#define SETDUTYCYCLE "SetDutyCycle::MW_PWM_SetDutyCycle(pin %" PRIu32 ",dutycycle %f%%);"
#else
#if ESB_BETA2_IOSERVER
#define CONFPIN "ConfigurePWMPin::MW_PWM_Open(pin %" PRIu32 ",counter %s ,frequency %" PRIu32 ",dutyCycle %" PRIu32 "%%);"
#else
#define CONFPIN "ConfigurePWMPin::MW_PWM_Open(pin %" PRIu32 ",frequency %" PRIu32 ",dutyCycle %" PRIu32 "%%);"
#endif
#define SETFREQUENCY "SetFrequency::MW_PWM_SetFrequency(pin %" PRIu32 ",frequency %" PRIu32 ");"
#define SETDUTYCYCLE "SetDutyCycle::MW_PWM_SetDutyCycle(pin %" PRIu32 ",dutycycle %" PRIu32 "%%);"
#endif
#define DISABLEPWMNOTIFICATION "DisablePWMNotification::MW_PWM_DisableNotification(pin %" PRIu32 ");"
#define ENABLEPWMNOTIFICATION "EnablePWMNotification::MW_PWM_EnableNotification(pin %" PRIu32 ", notification %s);"
#define SETPWMOUTPUTIDLE "SetPWMOutputIdle::MW_PWM_SetOutputToIdle(pin %" PRIu32 ");"
#define GETPWMOUTPUTSTATE "GetPWMOutputState::MW_PWM_GetOutputState(pin %" PRIu32 ");"
#define STOPPWM "StopPWM::MW_PWM_Stop(pin %" PRIu32 ");"
#define UNCONFIGPIN "UnconfigurePWMPin::MW_PWM_Close(pin %" PRIu32 ");"
#define RESETPIN1 "ResetPWMPins::MW_PWM_Stop(pin %" PRIu16 ");"
#define RESETPIN2 "ResetPWMPins::MW_PWM_Close(pin %" PRIu16 ");"
#define WARNING "Warning::%s approximated to uint32 for display."

#if (1 == FLOAT_ENABLED)
#define FLOAT_ENABLED_TYPECAST real32_T
#else
#define FLOAT_ENABLED_TYPECAST uint32_T
#endif

const char MSG_CONFIG_PWMPIN[] PGMKEYWORD = CONFPIN;
const char MSG_START_PWM[] PGMKEYWORD = STARTPWM;
const char MSG_SET_DUTYCYCLE[] PGMKEYWORD = SETDUTYCYCLE;
const char MSG_SET_FREQUENCY[] PGMKEYWORD = SETFREQUENCY;
const char MSG_DISABLE_PWM_NOTIFICATION[] PGMKEYWORD = DISABLEPWMNOTIFICATION;
const char MSG_ENABLE_PWM_NOTIFICATION[] PGMKEYWORD = ENABLEPWMNOTIFICATION;
const char MSG_SET_PWM_OUTPUT_IDLE[] PGMKEYWORD = SETPWMOUTPUTIDLE;
const char MSG_GET_PWM_OUTPUT_STATE[] PGMKEYWORD = GETPWMOUTPUTSTATE;
const char MSG_STOP_PWM[] PGMKEYWORD = STOPPWM;
const char MSG_UNCONFIG_PWMPIN[] PGMKEYWORD = UNCONFIGPIN;
const char MSG_RESETPWM_STOP[] PGMKEYWORD = RESETPIN1;
const char MSG_RESETPWM_CLOSE[] PGMKEYWORD = RESETPIN2;
const char MSG_WARNING[] PGMKEYWORD = WARNING;

#if ESB_BETA2_IOSERVER
const char counterModeType[NUMCOUNTERMODETYPES][MAXCOUNTERMODELENGTH] PGMKEYWORD = {"UpCounter", "DownCounter", "CenterAlignedCounter"};
const char notificationTypes[NUMNOTIFICATIONTYPES_BETA2][MAXNOTIFICATIONLENGTH_BETA2] PGMKEYWORD = {"NoNotification", "FallingEdge", "RisingEdge", "BothEdges", "CounterReachesZero", "CounterOverFlows", "CounterReachesZeroOrOverFlows"};
#else
const char notificationTypes[NUMNOTIFICATIONTYPES_BETA1][MAXNOTIFICATIONLENGTH_BETA1] PGMKEYWORD = {"NoNotification", "FallingEdge", "RisingEdge", "BothEdges"};
#endif

#endif
/* The IO_PWM_MODULES_MAX macros will be defined in peripheralIncludes.h */
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
PeripheralHandleMapType PWMMap[(IO_PWM_MODULES_MAX/8)+1] = {0};
#else
PeripheralHandleMapType PWMMap[IO_PWM_MODULES_MAX] = {NULL};

#endif


/***************************************************************************************************************************
 *              Configure the specified PWM pin with a specific frequency.
 *              Returns status as 0 after successful open.
 *              Returns status as 1 on failure.
 ****************************************************************************************************************************/

void configurePWMPin(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    real32_T frequency = 0;
    real32_T dutyCycle = 0;
    MW_Handle_Type PWMHandle = NULL;
#if ESB_BETA2_IOSERVER
    uint32_T counterMode = 0;
#if DEBUG_FLAG == 1
#ifdef __AVR_ARCH__
    int i;
    char cMode[MAXCOUNTERMODELENGTH];
#endif
#endif
#endif

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo,&payloadBufferRx[index],sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Reads 4 byte frequency from the input buffer */
    memcpy(&frequency,&payloadBufferRx[index],sizeof(real32_T));
    index += sizeof(real32_T);

    /* Reads 4 byte duty cycle from the input buffer */
    memcpy(&dutyCycle,&payloadBufferRx[index],sizeof(real32_T));
    index += sizeof(real32_T);

    /* Call the PWM configuration C layer SVD driver function */
#if ESB_BETA2_IOSERVER
    /* Reads 4 byte counter mode from the input buffer */
    memcpy(&counterMode, &payloadBufferRx[index], sizeof(uint32_T));
    index += sizeof(uint32_T);
#endif

#if DEBUG_FLAG == 1
    /* Print debug message */
#if ESB_BETA2_IOSERVER
#ifdef __AVR_ARCH__
    getProgramMemoryData(counterModeType[counterMode], cMode);
    debugPrint(MSG_CONFIG_PWMPIN, (uint32_T)pinNo, cMode, (FLOAT_ENABLED_TYPECAST)frequency, (FLOAT_ENABLED_TYPECAST)dutyCycle);
#else
    debugPrint(MSG_CONFIG_PWMPIN, (uint32_T)pinNo, counterModeType[counterMode], (FLOAT_ENABLED_TYPECAST)frequency, (FLOAT_ENABLED_TYPECAST)dutyCycle);
#endif
#else
    debugPrint(MSG_CONFIG_PWMPIN, pinNo, (FLOAT_ENABLED_TYPECAST)frequency, (FLOAT_ENABLED_TYPECAST)dutyCycle);
#endif
    #if (FLOAT_ENABLED!=1)
        if((frequency!=(uint32_T)frequency))
        {
            debugPrint(MSG_WARNING,"frequency");
        }
        if((dutyCycle!=(uint32_T)dutyCycle))
        {
            debugPrint(MSG_WARNING,"dutyCycle");
        }
    #endif
#endif

    /* Call the PWM configuration C layer SVD driver function */
#if ESB_BETA2_IOSERVER
    PWMHandle = MW_PWM_Open((uint32_T)pinNo, (MW_PWM_CounterModes_T)counterMode, (real_T)frequency, (real_T)dutyCycle);
#else
    PWMHandle = MW_PWM_Open((uint32_T)pinNo, (real_T)frequency, (real_T)dutyCycle);
#endif
    if ((MW_Handle_Type)NULL != PWMHandle)
    {
        /* Stores the handle if open was successful */
        setHandle(pinNo, PWMHandle, PWMMap);

        /* Returns status as 0 if open is successful */
        *status = 0;
    }
    else
    {
        /* Returns status as 1 if open is not successful */
        *status = 1;
    }

#if ESB_BETA1_IOSERVER
    MW_PWM_SetCounterMode(PWMHandle, TIM_COUNTERMODE_UP);
    MW_PWM_SetFrequency(PWMHandle, frequency);
    MW_PWM_SetDutyCycle(PWMHandle, dutyCycle);
#endif

}

/***************************************************************************************************************************
 *              Start generating the PWM on specified pin with a specific frequency and duty cycle.
 *              Returns status as 0 if pin is configured as PWM.
 *              Returns status as 1 if pin is not configured as PWM.
 ****************************************************************************************************************************/

void startPWM(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    MW_Handle_Type PWMHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo,&payloadBufferRx[index],sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Retrieve the PWM pin handle from the corresponding handle map */
    PWMHandle = getHandle(pinNo, PWMMap);

    if ((MW_Handle_Type)NULL != PWMHandle)
    {

#if DEBUG_FLAG == 1
        /* Print debug message */
        debugPrint(MSG_START_PWM,pinNo);
#endif

        MW_PWM_Start((MW_Handle_Type)PWMHandle);
        /* Always passes because SVD doesn't return status */
        /* Returns status as 0 if pin is configured as PWM */
        *status = 0;
    }
    else
    {
        /* Returns status as 1 if pin is not configured as PWM */
        *status = 1;
    }
}

/***************************************************************************************************************************
 *              Sets the duty cycle on the specified PWM pin.
 *              Returns status as 0 if pin is configured as PWM.
 *              Returns status as 1 if pin is not configured as PWM.
 ****************************************************************************************************************************/

void setDutyCycle(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    real32_T dutyCycle = 0;
    MW_Handle_Type PWMHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo,&payloadBufferRx[index],sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Reads 4 byte duty cycle from the input buffer */
    memcpy(&dutyCycle,&payloadBufferRx[index],sizeof(real32_T));
    index += sizeof(real32_T);


    /* Retrieve the PWM pin handle from the corresponding handle map */
    PWMHandle = getHandle(pinNo, PWMMap);

    if ((MW_Handle_Type)NULL != PWMHandle)
    {
#if DEBUG_FLAG == 1
        debugPrint(MSG_SET_DUTYCYCLE,pinNo,(FLOAT_ENABLED_TYPECAST)dutyCycle);
        #if (FLOAT_ENABLED!=1)
            if(dutyCycle!=((uint32_T)dutyCycle))
            {
                debugPrint(MSG_WARNING,"dutyCycle");
            }
        #endif
#endif
        MW_PWM_SetDutyCycle((MW_Handle_Type)PWMHandle,(real_T)dutyCycle);
        /* Always passes because SVD doesn't return status */
        /* Returns status as 0 if pin is configured as PWM */
        *status = 0;
    }
    else
    {
        /* Returns status as 1 if pin is not configured as PWM */
        *status = 1;
    }
}


/***************************************************************************************************************************
 *              Sets the frequency on the specified PWM pin.
 *              Returns status as 0 if pin is configured as PWM.
 *              Returns status as 1 if pin is not configured as PWM.
 ****************************************************************************************************************************/

void setFrequency(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    real32_T frequency = 0;
    MW_Handle_Type PWMHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo,&payloadBufferRx[index],sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Reads 4 byte frequency from the input buffer */
    memcpy(&frequency,&payloadBufferRx[index],sizeof(real32_T));
    index += sizeof(real32_T);


    /* Retrieve the PWM pin handle from the corresponding handle map */
    PWMHandle = getHandle(pinNo, PWMMap);

    if ((MW_Handle_Type)NULL != PWMHandle)
    {
#if DEBUG_FLAG == 1
        /* Print debug message */
            debugPrint(MSG_SET_FREQUENCY,pinNo,(FLOAT_ENABLED_TYPECAST)frequency);
            #if (FLOAT_ENABLED!=1)
                if(frequency!=((uint32_T)frequency))
                {
                    debugPrint(MSG_WARNING,"frequency");
                }
            #endif
#endif

        MW_PWM_SetFrequency((MW_Handle_Type)PWMHandle,(real_T)frequency);
        /* Always passes because SVD doesn't return status */
        /* Returns status as 0 if pin is configured as PWM */
        *status = 0;
    }
    else
    {
        /* Returns status as 1 if pin is not configured as PWM */
        *status = 1;
    }
}

/***************************************************************************************************************************
 *              Disable notifications on the channel.
 *              Returns status as 0 if pin is configured as PWM.
 *              Returns status as 1 if pin is not configured as PWM.
 ****************************************************************************************************************************/
void disablePWMNotification(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    MW_Handle_Type PWMHandle = NULL;
#if ESB_BETA2_IOSERVER
    uint32_T notification = 0;
#endif
    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo,&payloadBufferRx[index],sizeof(uint32_T));
    index += sizeof(uint32_T);


    /* Retrieve the PWM pin handle from the corresponding handle map */
    PWMHandle = getHandle(pinNo, PWMMap);

    if ((MW_Handle_Type)NULL != PWMHandle)
    {
#if ESB_BETA2_IOSERVER
        /* Reads 4 byte edge notification type from the input buffer */
        memcpy(&notification, &payloadBufferRx[index], sizeof(uint32_T));
        index += sizeof(uint32_T);
        MW_PWM_DisableNotification((MW_Handle_Type)PWMHandle,(MW_PWM_EdgeNotification_T)notification);
#else
#if DEBUG_FLAG == 1
        /* Print debug message */
        debugPrint(MSG_DISABLE_PWM_NOTIFICATION,pinNo);
#endif
        MW_PWM_DisableNotification((MW_Handle_Type)PWMHandle);
#endif
        /* Always passes because SVD doesn't return status */
        /* Returns status as 0 if pin is configured as PWM */
        *status = 0;
    }
    else
    {
        /* Returns status as 1 if pin is not configured as PWM */
        *status = 1;
    }
}

/***************************************************************************************************************************
 *              Enable notifications on the channel.
 *              Returns status as 0 if pin is configured as PWM.
 *              Returns status as 1 if pin is not configured as PWM.
 ****************************************************************************************************************************/
void enablePWMNotification(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    int8_T notification = 0;
    MW_Handle_Type PWMHandle = NULL;
#if DEBUG_FLAG == 1
#ifdef __AVR_ARCH__
    int i = 0;
#ifdef ESB_BETA2_IOSERVER
    char nType[MAXNOTIFICATIONLENGTH_BETA2];
#else
    char nType[MAXNOTIFICATIONLENGTH_BETA1];
#endif
#endif
#endif
    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo,&payloadBufferRx[index],sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Reads 1 byte edge notification type from the input buffer */
    notification = payloadBufferRx[index++];

    /* Retrieve the PWM pin handle from the corresponding handle map */
    PWMHandle = getHandle(pinNo, PWMMap);

    if ((MW_Handle_Type)NULL != PWMHandle)
    {

#if DEBUG_FLAG == 1
#ifdef ESB_BETA2_IOSERVER
        if((int8_T)MW_PWM_COUNTER_OVERFLOWS == notification)
        {
            notification = (int8_T)5; // For ESB Beta 2, the notification contains 3 more entries than the generic svd version. The enum for last 2 are 0x08 and 0x0B which need to be mapped to their corresponding index in array.
        }
        else if((int8_T)MW_PWM_COUNTER_REACHES_ZERO_OR_OVERFLOWS == notification)
        {
            notification = (int8_T)6;
        }
#endif
#ifdef __AVR_ARCH__
        getProgramMemoryData(notificationTypes[notification],nType);
        /* Print debug message */
        debugPrint(MSG_ENABLE_PWM_NOTIFICATION,(uint32_T)pinNo,nType);
#else
        /* Print debug message */
        debugPrint(MSG_ENABLE_PWM_NOTIFICATION,(uint32_T)pinNo,notificationTypes[notification]);
#endif

#endif
        MW_PWM_EnableNotification((MW_Handle_Type)PWMHandle,(MW_PWM_EdgeNotification_Type)notification);

        /* Always passes because SVD doesn't return status */
        /* Returns status as 0 if pin is configured as PWM */
        *status = 0;
    }
    else
    {
        /* Returns status as 1 if pin is not configured as PWM */
        *status = 1;
    }
}


/***************************************************************************************************************************
 *              Set PWM output to idle state
 *              Returns status as 0 if pin is configured as PWM.
 *              Returns status as 1 if pin is not configured as PWM.
 ****************************************************************************************************************************/
void setPWMOutputToIdle(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    MW_Handle_Type PWMHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo,&payloadBufferRx[index],sizeof(uint32_T));
    index += sizeof(uint32_T);


    /* Retrieve the PWM pin handle from the corresponding handle map */
    PWMHandle = getHandle(pinNo, PWMMap);

    if ((MW_Handle_Type)NULL != PWMHandle)
    {
#ifndef ESB_BETA2_IOSERVER
#if DEBUG_FLAG == 1
        /* Print debug message */
        debugPrint(MSG_SET_PWM_OUTPUT_IDLE,pinNo);
#endif
        MW_PWM_SetOutputToIdle((MW_Handle_Type)PWMHandle);
#endif
        /* Always passes because SVD doesn't return status */
        /* Returns status as 0 if pin is configured as PWM */
        *status = 0;
    }
    else
    {
        /* Returns status as 1 if pin is not configured as PWM */
        *status = 1;
    }
}



/***************************************************************************************************************************
 *              Get the PWM output status.
 *              Returns status as 0 if pin is configured as PWM. And Return value as 1 if pin is high else 0 if pin is low
 *              Returns status as 1 if pin is not configured as PWM. Return value is undefined.
 ****************************************************************************************************************************/
void getPWMOutputState(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint8_T index = 0;
    uint32_T pinNo = 0;
    boolean_T pinValue = 0;
    MW_Handle_Type PWMHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo,&payloadBufferRx[index],sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Retrieve the PWM pin handle from the corresponding handle map */
    PWMHandle = getHandle(pinNo, PWMMap);

    if ((MW_Handle_Type)NULL != PWMHandle)
    {
#ifndef ESB_BETA2_IOSERVER
#if DEBUG_FLAG == 1
        /* Print debug message */
        debugPrint(MSG_GET_PWM_OUTPUT_STATE,pinNo);
#endif
        pinValue = MW_PWM_GetOutputState((MW_Handle_Type)PWMHandle);
#endif
        /* Transmits the status of PWM pin back to the host */
        payloadBufferTx[(*peripheralDataSizeResponse)++] = (uint8_T)pinValue;

        /* Returns status as 0 if pin is configured as PWM */
        *status = 0;
    }
    else
    {
        /* Returns status as 1 if pin is not configured as PWM */
        *status = 1;
    }

}


/***************************************************************************************************************************
 *              Stop the PWM on the specified PWM pin.
 *              Returns status as 0 if pin is configured as PWM.
 *              Returns status as 1 if pin is not configured as PWM.
 ****************************************************************************************************************************/

void stopPWM(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{

    uint8_T index = 0;
    uint32_T pinNo = 0;
    MW_Handle_Type PWMHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo,&payloadBufferRx[index],sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Retrieve the PWM pin handle from the corresponding handle map */
    PWMHandle = getHandle(pinNo, PWMMap);

    if ((MW_Handle_Type)NULL != PWMHandle)
    {
#if DEBUG_FLAG == 1
        /* Print debug message */
        debugPrint(MSG_STOP_PWM,pinNo);
#endif
        /* stop the PWM on pin */
        MW_PWM_Stop((MW_Handle_Type)PWMHandle);
        /* Always passes because SVD doesn't return status */
        /* Returns status as 0 if pin is configured as PWM */
        *status = 0;
    }
    else
    {
        /* Returns status as 1 if pin is not configured as PWM */
        *status = 1;
    }

}


/***************************************************************************************************************************
 *              Unconfigures the specified PWM pin.
 *              Returns status as 0 if pin is unconfigured.
 *              Returns status as 1 if pin was not never configured for PWM operation.
 ****************************************************************************************************************************/

void unconfigurePWMPin(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{

    uint8_T index = 0;
    uint32_T pinNo = 0;
    MW_Handle_Type PWMHandle = NULL;

    /* Reads 4 byte pin number from the input buffer */
    memcpy(&pinNo,&payloadBufferRx[index],sizeof(uint32_T));
    index += sizeof(uint32_T);

    /* Retrieve the PWM pin handle from the corresponding handle map */
    PWMHandle = getHandle(pinNo, PWMMap);
    if ((MW_Handle_Type)NULL != PWMHandle)
    {
#if DEBUG_FLAG == 1
        /* Print debug message */
        debugPrint(MSG_UNCONFIG_PWMPIN,pinNo);
#endif
        /* close the PWM pin */
        MW_PWM_Close((MW_Handle_Type)PWMHandle);

        setHandle(pinNo, (MW_Handle_Type)NULL, PWMMap);
        /* Returns status as 0 if pin was configured for PWM operation */
        *status = 0;
    }
    else
    {
        /* Returns status as 1 if pin was not configured for PWM operation */
        *status = 1;
    }

}

/***************************************************************************************************************************
 *              Reset PWM pins.
 *              Returns status as 0
 ****************************************************************************************************************************/
void resetPWMPins(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status)
{
    uint16_T loopvar = 0;
    MW_Handle_Type IOHandle = NULL;

    for (loopvar = 0 ; loopvar < IO_PWM_MODULES_MAX ; loopvar++)
    {
        IOHandle = getHandle(loopvar, PWMMap);
        if ((MW_Handle_Type)NULL != IOHandle)
        {
#if DEBUG_FLAG == 1
            /* print debug message */
            debugPrint(MSG_RESETPWM_STOP, loopvar);
#endif

            MW_PWM_Stop((MW_Handle_Type)IOHandle);

            /* close the PWM pin */

#if DEBUG_FLAG == 1
            /* print debug message */
            debugPrint(MSG_RESETPWM_CLOSE, loopvar);
#endif

            MW_PWM_Close((MW_Handle_Type)IOHandle);

            setHandle(loopvar, (MW_Handle_Type)NULL, PWMMap);
        }
    }
    /* Returns status as 0 */
    *status = 0;
}
#endif
