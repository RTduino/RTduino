/*
 * Wrappers to make DAC functions in Arduino Due available with C linkage.
 * This allows C++ methods to be called from C code.
 *
/* Copyright 2015 The MathWorks, Inc. */
#if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
/* This will be run in Rapid Accelerator Mode */

    #define MW_DACWrite(a, b) (0)
#else
    #ifndef _MW_ANALOGWRITEDAC_H
    #define _MW_ANALOGWRITEDAC_H
    #include <inttypes.h>
    #include "rtwtypes.h"

    #ifdef __cplusplus
    extern "C" {
    #endif

    void MW_DACWrite(uint8_T channelFlag, uint32_T value);

    #ifdef __cplusplus
    }
    #endif
    #endif //_MW_ANALOGWRITEDAC_H
 #endif
