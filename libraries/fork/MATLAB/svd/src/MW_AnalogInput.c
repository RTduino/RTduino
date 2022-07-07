/* Copyright 2015-2017 The MathWorks, Inc. */

#include "MW_AnalogIn.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Create AnalogIn group with Channels and Conversion time */
MW_Handle_Type MW_AnalogInSingle_Open(uint32_T Pin)
{
    return (MW_Handle_Type)NULL;
}

/* Select trigger source for AnalogIn group to start conversion */
void MW_AnalogIn_SetTriggerSource(MW_Handle_Type AnalogInHandle, MW_AnalogIn_TriggerSource_Type TriggerType, uint32_T TriggerValue)
{
}

/* Enable Conversion complete notification */
void MW_AnalogIn_EnableNotification(MW_Handle_Type AnalogInHandle)
{
}

/* Disable notifications */
void MW_AnalogIn_DisableNotification(MW_Handle_Type AnalogInHandle)
{
}

/* Enable continuous conversion */
void MW_AnalogIn_EnableContConversion(MW_Handle_Type AnalogInHandle)
{
}

/* Set channel conversion priority */
void MW_AnalogIn_SetChannelConvRank(MW_Handle_Type AnalogInHandle, uint32_T Channel, uint32_T Rank)
{
}

/* Get status of AnalogIn conversion group */
MW_AnalogIn_Status_Type MW_AnalogIn_GetStatus(MW_Handle_Type AnalogInHandle)
{
    return MW_ANALOGIN_CONVERSION_COMPLETE;
}

/* Read channel conversion result */
void MW_AnalogInSingle_ReadResult(MW_Handle_Type AnalogInHandle, void * Result_ptr, MW_AnalogIn_ResultDataType_Type ResultDataType)
{
}

/* Start conversion */
void MW_AnalogIn_Start(MW_Handle_Type AnalogInHandle)
{
}

/* Stop conversion */
void MW_AnalogIn_Stop(MW_Handle_Type AnalogInHandle)
{
}

/* De-initialise */
void MW_AnalogIn_Close(MW_Handle_Type AnalogInHandle)
{
}

MW_Handle_Type MW_AnalogIn_GetHandle(uint32_T Pin)
{
    return (MW_Handle_Type)NULL;
}

#ifdef __cplusplus
}
#endif
