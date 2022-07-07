/*
 * Wrappers to make DAC functions in Arduino Due available with C linkage.
 * This allows C++ methods to be called from C code.
 *
/* Copyright 2015-2019 The MathWorks, Inc. */
#if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
/* This will be run in Rapid Accelerator Mode */
/*do nothing */

#else

    #include "Arduino.h"
    #include "MW_analogWriteDAC.h"
    #include "io_wrappers.h"

    extern "C" {
        void __cxa_pure_virtual(void);
    }

    extern "C" void MW_DACWrite(uint8_T channelFlag, uint32_T value) {
        #if defined(_ROTH_DUE_)
        /* dividing the data by 16 to adjust for upscaling by 16 in Arduino analogWrite API */
            analogWrite(((channelFlag == 0)? DAC0 : DAC1), (value>>4));
        #elif defined(_ROTH_MKR1000_)
        /* dividing the data by 4 to adjust for upscaling by 4 in Arduino analogWrite API */
            analogWrite(DAC0, (value>>2));
        #elif defined(_ROTH_MKRZERO_) || defined(_ROTH_MKRWIFI1010_)
        /* dividing the data by 4 to adjust for upscaling by 4 in Arduino analogWrite API */
            analogWrite(15u, (value>>2));
        #endif
    }

#endif
