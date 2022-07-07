/*
 * Wrappers to make SPI functions available with C linkage. This allows C++
 * methods to be called from C code.
 *
 * Copyright 2015 The MathWorks, Inc. */

#if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
/* This will be run in Rapid Accelerator Mode */

    #define MW_SSpinSetup(x) (0)
    #define MW_SPIwriteReadDueSetup(x) (0)
    #define MW_SPIwriteReadAVRSetup() (0)
    #define MW_SPIwriteReadLoop(a, b, c, d, e) (0)
#else

    #ifndef _MW_SPIWRITEREAD_H
    #define _MW_SPIWRITEREAD_H
    #include <inttypes.h>
    #include "rtwtypes.h"

    #ifdef __cplusplus
    extern "C" {
    #endif

    void MW_SSpinSetup(uint8_T slaveSelectPin);

    #ifdef _ROTH_DUE_
    void MW_SPIwriteReadDueSetup(uint8_T slaveSelectPin);
    #else
    void MW_SPIwriteReadAVRSetup(void);
    #endif
    void MW_SPIwriteReadLoop(uint8_T slaveSelectPin, void *input, uint8_T datalength, uint8_T dataType,void *output);

    #ifdef __cplusplus
    }
    #endif
    #endif //_MW_SPIWRITEREAD_H
#endif
