#ifndef ARDUINO_EXTINT_LCT_H
#define ARDUINO_EXTINT_LCT_H

#if defined(_RUNONTARGETHARDWARE_BUILD_)
/* This will be called by the target compiler */

#include "Arduino.h"

#else
/* This will be compiled by MATLAB to create the Simulink block */
/* and for rapid accelerator mode as well */

#define attachInterrupt(a,b,c) (0)

#endif
#endif /*arduino_extint_lct.h*/
