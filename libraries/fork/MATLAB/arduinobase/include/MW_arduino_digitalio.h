#ifndef _MW_ARDUINO_DIGITALIO_H_
#define _MW_ARDUINO_DIGITALIO_H_
#include "rtwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(MATLAB_MEX_FILE) || defined(RSIM_PARAMETER_LOADING) ||  defined(RSIM_WITH_SL_SOLVER))
 /* This will be run in Rapid Accelerator Mode */
 #define DIN_INIT(pin) digitalIOSetup(pin, 0)
 #define DOUT_INIT(pin) digitalIOSetup(pin, 1)
 #define digitalIOSetup(pin, mode)  (0)
 #define writeDigitalPin(pin, val)  (0)
 #define readDigitalPin(pin)        (0)
#else
 #define DIN_INIT(pin) digitalIOSetup(pin, 0)
 #define DOUT_INIT(pin) digitalIOSetup(pin, 1)
 void digitalIOSetup(uint8_T pin, uint8_T mode);
 void writeDigitalPin(uint8_T pin, boolean_T val);
 boolean_T readDigitalPin(uint8_T pin);
#endif

#ifdef __cplusplus
}
#endif
#endif
