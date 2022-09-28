/*
 * Wrappers to make I2C functions available with C linkage. This allows C++
 * methods to be called from C code.
 *
 * Copyright 2015 The MathWorks, Inc. */

#if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
/* This will be run in Rapid Accelerator Mode */

    #define MW_i2cSetup() (0)
    #define MW_i2cWriteLoop(a, b, c, d, e) (0)
    #define MW_i2cReadLoop(a, b, c, d, e) (0)

#else

    #ifndef _MW_ARDUINOI2C_H
    #define _MW_ARDUINOI2C_H
    #include <inttypes.h>
    #include "rtwtypes.h"


    #ifdef __cplusplus
    extern "C" {
    #endif

    void MW_i2cSetup(void);

    void MW_i2cWriteLoop(uint8_T SlaveAddress, uint8_T RegisterAddressFlag, uint8_T RegisterAddress, uint8_T *data, size_t quantity);

    void MW_i2cReadLoop(uint8_T SlaveAddress, uint8_T RegisterAddressFlag, uint8_T RegisterAddress, uint8_T quantity, uint8_T *data);

    #ifdef __cplusplus
    }
    #endif
    #endif //_MW_ARDUINOI2C_H

#endif
