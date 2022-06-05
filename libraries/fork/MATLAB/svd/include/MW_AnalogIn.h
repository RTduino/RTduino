/* Copyright 2015-2017 The MathWorks, Inc. */

#ifndef __MW_ANALOG_IN_H__
#define __MW_ANALOG_IN_H__

#include "MW_SVD.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MW_ANALOGIN_SOFTWARE_TRIGGER = 0,
    MW_ANALOGIN_EXTERNAL_TRIGGER
} MW_AnalogIn_TriggerSource_Type;

typedef enum {
    MW_ANALOGIN_IDLE = 0,
    MW_ANALOGIN_BUSY,
    MW_ANALOGIN_CONVERSION_COMPLETE,
    MW_ANALOGIN_ERROR
} MW_AnalogIn_Status_Type;

typedef enum {
    MW_ANALOGIN_INT8 = 0,
    MW_ANALOGIN_UINT8,
    MW_ANALOGIN_INT16,
    MW_ANALOGIN_UINT16,
    MW_ANALOGIN_INT32,
    MW_ANALOGIN_UINT32,
    MW_ANALOGIN_FLOAT,
    MW_ANALOGIN_DOUBLE
} MW_AnalogIn_ResultDataType_Type;

/* Create AnalogIn group with Channels and Conversion time */
void MW_AnalogIn_Open(uint32_T GroupId, uint32_T * ChannelsId_ptr, real32_T * ConvTimeInSecs_ptr, uint8_T NumberOfChannels);
/* Create AnalogIn */
MW_Handle_Type MW_AnalogInSingle_Open(uint32_T Pin);
/* Select trigger source for AnalogIn group to start conversion */
void MW_AnalogIn_SetTriggerSource(MW_Handle_Type AnalogInHandle, MW_AnalogIn_TriggerSource_Type TriggerType, uint32_T TriggerValue);
/* Enable Conversion complete notification */
void MW_AnalogIn_EnableNotification(MW_Handle_Type AnalogInHandle);
/* Disable notifications */
void MW_AnalogIn_DisableNotification(MW_Handle_Type AnalogInHandle);
/* Enable continuous conversion */
void MW_AnalogIn_EnableContConversion(MW_Handle_Type AnalogInHandle);
/* Set channel conversion priority */
void MW_AnalogIn_SetChannelConvRank(MW_Handle_Type AnalogInHandle, uint32_T Channel, uint32_T Rank);
/* Get status of AnalogIn conversion group */
MW_AnalogIn_Status_Type MW_AnalogIn_GetStatus(MW_Handle_Type AnalogInHandle);
/* Read channel conversion result */
void MW_AnalogIn_ReadResult(uint32_T GroupId, void * Result_ptr, uint8_T NumberOfChannels, MW_AnalogIn_ResultDataType_Type ResultDataType);
/* Read single channel conversion result */
void MW_AnalogInSingle_ReadResult(MW_Handle_Type AnalogInHandle, void * Result_ptr, MW_AnalogIn_ResultDataType_Type ResultDataType);
/* Start conversion */
void MW_AnalogIn_Start(MW_Handle_Type AnalogInHandle);
/* Stop conversion */
void MW_AnalogIn_Stop(MW_Handle_Type AnalogInHandle);
/* Release Analog Input conversion */
void MW_AnalogIn_Close(MW_Handle_Type AnalogInHandle);
/* Return Analog Input handle. Required when only read result mode */
MW_Handle_Type MW_AnalogIn_GetHandle(uint32_T Pin);

#ifdef __cplusplus
}
#endif

#endif /* __MW_ANALOG_IN_H__ */
