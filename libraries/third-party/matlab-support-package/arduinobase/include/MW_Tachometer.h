/*
 * Wrappers to make Tachometer functions available with C linkage.
 * This allows C++ methods to be called from C code.
 *
/* Copyright 2018 The MathWorks, Inc. */
#ifndef _MW_ARDUINO_TACHOMETER_H_
#define _MW_ARDUINO_TACHOMETER_H_
#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER))
 /* This will be run in Rapid Accelerator Mode */
 #define MW_Tachometer_Init(pin, mode)  (0)
 #define MW_Tachometer_GetSpeed()  (0)
 #define countISR() (0)
#else
 void countISR(void);
 void MW_Tachometer_Init(uint8_T pin, uint8_T mode);
 uint32_T MW_Tachometer_GetSpeed(void);
#endif

#ifdef __cplusplus
}
#endif
#endif //_MW_ARDUINO_TACHOMETER_H_
