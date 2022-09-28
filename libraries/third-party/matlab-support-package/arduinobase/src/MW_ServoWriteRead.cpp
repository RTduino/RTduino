/*
 * Wrappers to make Servo functions available with C linkage. This allows C++
 * methods to be called from C code.
 *
 * Copyright 2017 The MathWorks, Inc. */

#if ( defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER) )
/* This will be run in Rapid Accelerator Mode */
/* Do Nothing*/
#else

    #include <inttypes.h>
    #include "Arduino.h"
    #include "MW_ServoWriteRead.h"

    #if (defined(_RTT_NUMSERVOS_) && (_RTT_NUMSERVOS_ != 0))
    #include "Servo.h"
    #endif

    #if (defined(_RTT_NUMSERVOS_) && (_RTT_NUMSERVOS_ != 0))
        Servo myservos[_RTT_NUMSERVOS_];

        extern "C" void MW_servoAttach(uint8_t nServo, uint8_t pinNumber)
        {
            myservos[nServo].attach(pinNumber);
        }

        extern "C" void MW_servoWrite(uint8_t nServo, uint8_t inValue)
        {
            myservos[nServo].write(inValue);
        }

        extern "C" int MW_servoRead(uint8_t nServo)
        {
            return myservos[nServo].read();
        }
    #endif
#endif
