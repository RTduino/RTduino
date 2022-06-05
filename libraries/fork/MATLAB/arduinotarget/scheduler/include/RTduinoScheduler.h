/* Arduino Bareboard scheduler
 *
 * Copyright 2013-2015 The MathWorks, Inc.
 */

#ifndef _RTDUINOSCHEDULER_H_
#define _RTDUINOSCHEDULER_H_

#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

void configureArduinoAVRTimer(void);
void disable_rt_OneStep(void);
boolean MW_Arduino_Loop(void);
void rt_OneStep(void);

#ifdef __cplusplus
}
#endif

#endif // _RTDUINOSCHEDULER_H_
