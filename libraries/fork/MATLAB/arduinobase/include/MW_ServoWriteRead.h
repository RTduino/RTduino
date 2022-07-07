/*
 * Wrappers to make Servo functions available with C linkage. This allows C++
 * methods to be called from C code.
 *
 * Copyright 2017 The MathWorks, Inc. */

#ifndef _MW_SERVOWRITEREAD_H_
#define _MW_SERVOWRITEREAD_H_

#if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
/* This will be run in Rapid Accelerator Mode */

    #define MW_servoAttach(a,b) (0)
    #define MW_servoWrite(a, b) (0)
    #define MW_servoRead(a) (0)

#else
    #include <inttypes.h>
    #include <stdio.h> /* for size_t */

    #ifdef __cplusplus
    extern "C" {
    #endif

    #if _RTT_NUMSERVOS_ != 0
    void MW_servoAttach(uint8_t nServo, uint8_t pinNumber);
    void MW_servoWrite(uint8_t nServo, uint8_t inValue);
    int MW_servoRead(uint8_t nServo);
    #endif

    #ifdef __cplusplus
    }
    #endif
#endif
#endif
