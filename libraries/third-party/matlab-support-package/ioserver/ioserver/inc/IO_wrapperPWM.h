/**
* @file IO_wrapperPWM.h
*
* Contains declarations of functions used by the PWM IO Wrapper
*
* @Copyright 2017-2018 The MathWorks, Inc.
*
*/
#ifndef IO_WRAPPERPWM_H
#define IO_WRAPPERPWM_H

#include "IO_peripheralInclude.h"
#ifdef ESB_BETA2_IOSERVER
#include "mw_pwm.h"
#if DEBUG_FLAG
#define NUMCOUNTERMODETYPES 3
#define MAXCOUNTERMODELENGTH 21
#endif
#else
#include "MW_PWM.h"
#endif
#if DEBUG_FLAG
#ifdef ESB_BETA2_IOSERVER
#define NUMNOTIFICATIONTYPES_BETA2 7
#define MAXNOTIFICATIONLENGTH_BETA2 30
#define NUMCOUNTERMODETYPES 3
#define MAXCOUNTERMODELENGTH 21
#else
#define NUMNOTIFICATIONTYPES_BETA1 4
#define MAXNOTIFICATIONLENGTH_BETA1 23
#endif
#endif


/* Configures a PWM pin */
void configurePWMPin(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Start PWM */
void startPWM(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Sets duty cycle on a PWM pin */
void setDutyCycle(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Sets frequency on a PWM pin */
void setFrequency(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Disable notifications on the channel */
void disablePWMNotification(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Enable notifications on the channel */
void enablePWMNotification(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Set PWM output to idle state */
void setPWMOutputToIdle(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Get the PWM output status */
void getPWMOutputState(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Stop PWM */
void stopPWM(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* Unconfigures a PWM pin */
void unconfigurePWMPin(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);
/* stop all PWM pins - Called on clearing the object in the host */
void resetPWMPins(uint8_T* payloadBufferRx, uint8_T* payloadBufferTx, uint16_T* peripheralDataSizeResponse, uint8_T* status);


#endif /*IO_WRAPPERPWM_H */

