/* Copyright 2015-2017 The MathWorks, Inc. */

#ifndef __MW_PWM_H__
#define __MW_PWM_H__

#include "MW_SVD.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Notificaion types */
typedef enum {
    MW_PWM_NO_NOTIFICATION = 0,
    MW_PWM_FALLING_EDGE,
    MW_PWM_RISING_EDGE,
    MW_PWM_BOTH_EDGES
} MW_PWM_EdgeNotification_Type;


/* PWM Initialisation selected by the pinNumber (PWM Channel) */
MW_Handle_Type MW_PWM_Open(uint32_T pin, real_T frequency, real_T dutyCycle);
/* Start PWM */
void MW_PWM_Start(MW_Handle_Type PWMPinHandle);
/* Set the duty cycle or pulse width for the PWM signal */
void MW_PWM_SetDutyCycle(MW_Handle_Type PWMPinHandle, real_T dutyCycle);
/* Set the PWM signal frequency */
void MW_PWM_SetFrequency(MW_Handle_Type PWMPinHandle, real_T frequency);
/* Disable notifications on the channel */
void MW_PWM_DisableNotification(MW_Handle_Type PWMPinHandle);
/* Enable notifications on the channel */
void MW_PWM_EnableNotification(MW_Handle_Type PWMPinHandle, MW_PWM_EdgeNotification_Type Notification);
/* Set PWM output to idle state */
void MW_PWM_SetOutputToIdle(MW_Handle_Type PWMPinHandle);
/* Get the PWM output status */
boolean_T MW_PWM_GetOutputState(MW_Handle_Type PWMPinHandle);
/* Stop PWM */
void MW_PWM_Stop(MW_Handle_Type PWMPinHandle);
/* Close PWM */
void MW_PWM_Close(MW_Handle_Type PWMPinHandle);

#ifdef __cplusplus
}
#endif

#endif /* __MW_PWM_H__ */
