/*
 * Wrappers to make Ultrasonic sensor functions available with C linkage.
 * This allows C++ methods to be called from C code.
 *
/* Copyright 2018 The MathWorks, Inc. */
#ifndef _MW_ULTRASONIC_H
#define _MW_ULTRASONIC_H
    #if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
    /* This will be run in Rapid Accelerator Mode */
        #define MW_UltrasonicSetup(a, b) (0)
        #define MW_UltrasonicRead(a,b,c,d,e) (0)
    #else

        #include <inttypes.h>
        #include "rtwtypes.h"

        #ifdef __cplusplus
        extern "C" {
        #endif
        void MW_UltrasonicSetup(uint8_T TrigPin, uint8_T EchoPin);
        void MW_UltrasonicRead(uint32_T *duration, uint8_T hasTrig, uint8_T TrigPin, uint8_T EchoPin, uint8_T Trigger_Pulse_Duration);

        #ifdef __cplusplus
        }
        #endif

     #endif
#endif //_MW_ULTRASONIC_H
